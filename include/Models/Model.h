#pragma once

#include "Camera.h"
#include "Mesh.h"
#include "Shader.h"
#include "Vertex.h"

#include <memory>
#include <string>

class Model {
  public:
    explicit Model(const char* file);
    virtual ~Model() = default;

    void draw(Shader& shader, Camera& camera);

    static std::unique_ptr<Model> load(const char* file);

    void translate(const glm::vec3& translation);
    void rotate(float AngleInRadians, const glm::vec3& rotationAxis);
    void scale(const glm::vec3& scaleFactor);
    void scale(float scaleFactor);

    void setPosition(const glm::vec3& position);
    void setRotation(const glm::quat& rotation);
    void setScale(const glm::vec3& scale);
    void setScale(float scaleFactor);

    [[nodiscard]] glm::vec3 getPosition() const;
    [[nodiscard]] glm::quat getRotation() const;
    [[nodiscard]] glm::vec3 getScale() const;
    [[nodiscard]] glm::mat4 getModelMatrix();

  protected:
    std::string file;
    std::vector<Mesh> meshes;

    // Model transform properties
    glm::vec3 currentPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::quat currentRotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 currentScale = glm::vec3(1.0f, 1.0f, 1.0f);

    bool translationChanged = true;
    glm::mat4 cachedTranslationMatrix = glm::mat4(1.0f);

    std::vector<Vertex> assembleVertices(std::vector<glm::vec3> positions, std::vector<glm::vec3> normals, std::vector<glm::vec2> texUVs);
};
