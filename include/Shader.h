#pragma once

#include <cerrno>
#include <fstream>
#include <glad/glad.h>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

class Shader {
  public:
    GLuint ID;
    Shader(const char* vertexFile, const char* fragmentFile);

    void activate() const;
    void deleteFromMemory() const;

  private:
    void compileErrors(unsigned int shader, const char* type);
};
