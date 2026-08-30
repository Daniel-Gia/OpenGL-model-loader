#include "Models/GLTF.h"

#include "Vertex.h"

#include <fastgltf/glm_element_traits.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdexcept>
#include <string>

GLTFModel::GLTFModel(const char* file) : Model(file), m_directory(std::filesystem::path(file).parent_path()) {
    std::filesystem::path path(file);

    auto data = fastgltf::GltfDataBuffer::FromPath(path);

    if (data.error() != fastgltf::Error::None) {
        throw std::runtime_error("Failed to load glTF file: " + path.string());
    }

    fastgltf::Parser parser;

    auto asset = parser.loadGltf(data.get(), path.parent_path(), fastgltf::Options::LoadExternalBuffers | fastgltf::Options::GenerateMeshIndices);

    if (asset.error() != fastgltf::Error::None) {
        throw std::runtime_error("Failed to parse glTF file: " + path.string());
    }

    m_asset = std::move(asset.get());
    loadScene();
}

void GLTFModel::loadScene() {
    if (m_asset.scenes.empty()) {
        throw std::runtime_error("glTF file contains no scenes.");
    }

    std::size_t sceneIndex = m_asset.defaultScene.value_or(0);

    if (sceneIndex >= m_asset.scenes.size()) {
        throw std::runtime_error("Invalid default scene index.");
    }

    const fastgltf::math::fmat4x4 Initial(1.0f);

    fastgltf::iterateSceneNodes(m_asset, sceneIndex, Initial, [this](fastgltf::Node& node, const fastgltf::math::fmat4x4& transform) {
        glm::mat4 glmTransform = glm::make_mat4(transform.data());

        if (node.meshIndex.has_value()) {
            const fastgltf::Mesh& mesh = m_asset.meshes[*node.meshIndex];

            loadMesh(mesh, glmTransform);
        }
    });
}

void GLTFModel::loadMesh(const fastgltf::Mesh& mesh, const glm::mat4& transform) {
    for (const auto& primitive : mesh.primitives) {
        if (primitive.type != fastgltf::PrimitiveType::Triangles) {
            continue;
        }

        // POSITION
        const auto& positionAttribute = primitive.findAttribute("POSITION");
        if (positionAttribute == primitive.attributes.end()) {
            throw std::runtime_error("Primitive is missing POSITION attribute.");
        }

        const fastgltf::Accessor& positionAccessor = m_asset.accessors[positionAttribute->accessorIndex];
        std::vector<glm::vec3> positions(positionAccessor.count);
        fastgltf::copyFromAccessor<glm::vec3>(m_asset, positionAccessor, positions.data());

        // NORMAL
        std::vector<glm::vec3> normals;
        const auto& normalAttribute = primitive.findAttribute("NORMAL");

        if (normalAttribute != primitive.attributes.end()) {
            const fastgltf::Accessor& normalAccessor = m_asset.accessors[normalAttribute->accessorIndex];
            normals.resize(normalAccessor.count);
            fastgltf::copyFromAccessor<glm::vec3>(m_asset, normalAccessor, normals.data());
        } else {
            normals.resize(positions.size(), glm::vec3(0.0f, 0.0f, 0.0f));
        }

        // TEXTURE COORDINATES
        std::vector<glm::vec2> texUVs;
        const auto& texCoordAttribute = primitive.findAttribute("TEXCOORD_0");

        if (texCoordAttribute != primitive.attributes.end()) {
            const fastgltf::Accessor& texCoordAccessor = m_asset.accessors[texCoordAttribute->accessorIndex];
            texUVs.resize(texCoordAccessor.count);
            fastgltf::copyFromAccessor<glm::vec2>(m_asset, texCoordAccessor, texUVs.data());
        } else {
            texUVs.resize(positions.size(), glm::vec2(0.0f, 0.0f));
        }

        // INDICES
        std::vector<GLuint> indices;

        if (primitive.indicesAccessor.has_value()) {
            const fastgltf::Accessor& indexAccessor = m_asset.accessors[*primitive.indicesAccessor];
            indices.resize(indexAccessor.count);
            fastgltf::copyFromAccessor<GLuint>(m_asset, indexAccessor, indices.data());
        } else {
            indices.resize(positions.size());

            for (std::size_t i = 0; i < positions.size(); ++i) {
                indices[i] = static_cast<GLuint>(i);
            }
        }

        std::vector<Vertex> vertices = assembleVertices(positions, normals, texUVs);
        std::vector<Texture> textures = loadMaterial(primitive);

        meshes.emplace_back(vertices, indices, textures, transform);
    }
}

std::vector<Texture> GLTFModel::loadMaterial(const fastgltf::Primitive& primitive) {
    std::vector<Texture> textures;

    if (!primitive.materialIndex.has_value()) {
        return textures;
    }

    std::size_t materialIndex = *primitive.materialIndex;

    if (materialIndex >= m_asset.materials.size()) {
        return textures;
    }

    const fastgltf::Material& material = m_asset.materials[materialIndex];

    if (material.pbrData.baseColorTexture.has_value()) {
        const fastgltf::TextureInfo& textureInfo = *material.pbrData.baseColorTexture;
        Texture texture = loadTexture(textureInfo, "baseColor");
        textures.push_back(texture);
    }

    if (material.pbrData.metallicRoughnessTexture.has_value()) {
        const fastgltf::TextureInfo& textureInfo = *material.pbrData.metallicRoughnessTexture;
        Texture texture = loadTexture(textureInfo, "metallicRoughness");
        textures.push_back(texture);
    }

    return textures;
}

Texture GLTFModel::loadTexture(const fastgltf::TextureInfo& textureInfo, const char* type) {
    std::filesystem::path texturePath = getTexturePath(textureInfo);

    if (!std::filesystem::exists(texturePath)) {
        throw std::runtime_error("Texture file not found: " + texturePath.string());
    }

    return {texturePath.string().c_str(), type};
}

std::filesystem::path GLTFModel::getTexturePath(const fastgltf::TextureInfo& textureInfo) const {
    if (textureInfo.textureIndex >= m_asset.textures.size()) {
        throw std::runtime_error("Invalid texture index.");
    }

    const fastgltf::Texture& texture = m_asset.textures[textureInfo.textureIndex];

    if (!texture.imageIndex.has_value()) {
        throw std::runtime_error("Texture does not have an image.");
    }

    const fastgltf::Image& image = m_asset.images[*texture.imageIndex];
    const auto* uri = std::get_if<fastgltf::sources::URI>(&image.data);

    if (uri == nullptr) {
        throw std::runtime_error("Image data is not a URI.");
    }

    if (!uri->uri.isLocalPath()) {
        throw std::runtime_error("Image URI is not a local path.");
    }

    return m_directory / uri->uri.fspath();
}
