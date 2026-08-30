#pragma once

#include "Vertex.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

class VBO {
  public:
    GLuint ID;
    VBO(std::vector<Vertex>& vertices);

    void bind() const;
    void unbind();
    void deleteFromMemory();
};
