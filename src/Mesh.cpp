#include "Mesh.h"

#include "Vertex.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices, std::vector<Texture>& textures, glm::mat4 meshModelMatrix) {
    this->meshModelMatrix = meshModelMatrix;
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;

    vao.bind();
    VBO vbo(vertices);
    EBO ebo(indices);

    // Links VBO attributes
    vao.linkAttrib(vbo, 0, 3, GL_FLOAT, sizeof(Vertex), nullptr);
    vao.linkAttrib(vbo, 1, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(3 * sizeof(float))); // NOLINT
    vao.linkAttrib(vbo, 2, 3, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(6 * sizeof(float))); // NOLINT
    vao.linkAttrib(vbo, 3, 2, GL_FLOAT, sizeof(Vertex), reinterpret_cast<void*>(9 * sizeof(float))); // NOLINT

    // Unbind so we dont accidentally modify them
    vao.unbind();
    vbo.unbind();
    ebo.unbind();
}

void Mesh::draw(Shader& shader, Camera& camera, glm::mat4 globalModelMatrix) {
    // Bind the shader and the VAO
    shader.activate();
    vao.bind();

    // how many of each type of textures we have
    unsigned int numBaseColor = 0;
    unsigned int numMetallicRoughness = 0;

    bool hasBaseColorTex = false;
    bool hasMetallicRoughnessTex = false;

    for (auto& texture : textures) {
        std::string num;
        std::string type = texture.type;

        if (type == "baseColor") {
            num = std::to_string(numBaseColor++);
        } else if (type == "metallicRoughness") {
            num = std::to_string(numMetallicRoughness++);
        }

        texture.setUniformSlot(shader, (type + num).c_str());
        texture.bind();

        if (type == "baseColor") {
            hasBaseColorTex = true;
        } else if (type == "metallicRoughness") {
            hasMetallicRoughnessTex = true;
        }
    }

    Texture::setUniformHasTexture(shader, "hasBaseColorTex", hasBaseColorTex);
    Texture::setUniformHasTexture(shader, "hasMetallicRoughnessTex", hasMetallicRoughnessTex);

    camera.exportPositionToShader(shader, "camPos");
    camera.exportMatrixToShader(shader, "camMatrix");

    glm::mat4 matrix = globalModelMatrix * meshModelMatrix;

    // Push the matrices to the shader
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(matrix));

    // Draw the actual mesh
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
}
