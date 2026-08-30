
#pragma once

#include "VBO.h"

#include <glad/glad.h>

class VAO {
  public:
    GLuint ID;
    VAO();

    void linkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);
    void bind() const;
    void unbind();
    void deleteFromMemory();
};
