#include "Models/Model.h"

#include "Models/GLTF.h"
#include "Vertex.h"

#include <filesystem>
#include <stdexcept>

Model::Model(const char* file) : file(file) {}

void Model::draw(Shader& shader, Camera& camera) {
    glm::mat4 globalModelMatrix = getModelMatrix();

    for (auto& mesh : meshes) {
        mesh.draw(shader, camera, globalModelMatrix);
    }
}

std::unique_ptr<Model> Model::load(const char* file) {
    std::filesystem::path path(file);

    if (!path.has_extension()) {
        throw std::runtime_error("File has no extension");
    }

    if (path.extension() == ".gltf") {
        return std::make_unique<GLTFModel>(file);
    }

    throw std::runtime_error("Unsupported file format" + path.extension().string());
}

std::vector<Vertex> Model::assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs) {
    std::vector<Vertex> vertices;
    vertices.reserve(positions.size());

    for (int i = 0; i < positions.size(); i++) {
        Vertex vertex = {positions[i], normals[i], glm::vec3(1.0f, 1.0f, 1.0f), texUVs[i]};

        vertices.push_back(vertex);
    }
    return vertices;
}

void Model::translate(const glm::vec3& translation) {
    currentPosition += translation;
    translationChanged = true;
}

void Model::rotate(const float AngleInRadians, const glm::vec3& rotationAxis) {
    glm::quat rot = glm::angleAxis(AngleInRadians, glm::normalize(rotationAxis));
    currentRotation = rot * currentRotation;
    translationChanged = true;
}

void Model::scale(const glm::vec3& scaleFactor) {
    currentScale *= scaleFactor;
    translationChanged = true;
}

void Model::scale(float scaleFactor) {
    currentScale *= glm::vec3(scaleFactor);
    translationChanged = true;
}

void Model::setPosition(const glm::vec3& position) {
    currentPosition = position;
    translationChanged = true;
}

void Model::setRotation(const glm::quat& rotation) {
    currentRotation = rotation;
    translationChanged = true;
}

void Model::setScale(const glm::vec3& scale) {
    currentScale = scale;
    translationChanged = true;
}

void Model::setScale(float scaleFactor) {
    currentScale = glm::vec3(scaleFactor);
    translationChanged = true;
}

glm::vec3 Model::getPosition() const {
    return currentPosition;
}

glm::quat Model::getRotation() const {
    return currentRotation;
}

glm::vec3 Model::getScale() const {
    return currentScale;
}

glm::mat4 Model::getModelMatrix() {
    if (translationChanged) {
        glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), currentPosition);
        glm::mat4 rotationMatrix = glm::toMat4(currentRotation);
        glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), currentScale);

        cachedTranslationMatrix = translationMatrix * rotationMatrix * scaleMatrix;
        translationChanged = false;
    }
    return cachedTranslationMatrix;
}
