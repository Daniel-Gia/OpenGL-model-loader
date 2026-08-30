#pragma once

#include <glad/glad.h>
#include <vector>

class EBO {
  public:
    GLuint ID;
    EBO(std::vector<GLuint>& indices);

    void bind() const;
    void unbind();
    void deleteFromMemory();
};
