#pragma once

#include "Models/Model.h"

#include <fastgltf/core.hpp>
#include <filesystem>
#include <vector>

class GLTFModel : public Model {
  public:
    explicit GLTFModel(const char* file);

  private:
    fastgltf::Asset m_asset;
    std::filesystem::path m_directory;

    void loadScene();
    void loadNode(const fastgltf::Node& node, const glm::mat4& transform);
    void loadMesh(const fastgltf::Mesh& mesh, const glm::mat4& transform);

    std::vector<Texture> loadMaterial(const fastgltf::Primitive& primitive);
    Texture loadTexture(const fastgltf::TextureInfo& textureInfo, const char* type);

    [[nodiscard]] std::filesystem::path getTexturePath(const fastgltf::TextureInfo& textureInfo) const;
};
