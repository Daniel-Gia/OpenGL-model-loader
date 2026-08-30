#pragma once

#include "Camera.h"
#include "EBO.h"
#include "Shader.h"
#include "Texture.h"
#include "VAO.h"
#include "VBO.h"
#include "Vertex.h"

#include <glm/glm.hpp>
#include <string>
#include <vector>

class Mesh {
  public:
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;
    std::vector<Texture> textures;

    glm::mat4 meshModelMatrix = glm::mat4(1.0f);

    VAO vao;

    Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, glm::mat4 meshModelMatrix = glm::mat4(1.0f));

    void draw(Shader& shader, Camera& camera, glm::mat4 globalModelMatrix = glm::mat4(1.0f));
};
