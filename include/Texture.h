#pragma once

#include "Shader.h"

#include <glad/glad.h>
#include <stb_image.h>

class Texture {
  public:
    GLuint ID;
    const char* type = nullptr; // baseColor, metallicRoughness
    GLuint slot = 0;

    Texture(const char* image, const char* texType);

    static void setUniformHasTexture(Shader& shader, const char* uniform, bool hasTexture);

    // Assigns a texture unit to a texture
    void setUniformSlot(Shader& shader, const char* uniform) const;

    void bind() const;
    void unbind();
    void deleteFromMemory();

  private:
    static GLuint getSlotByType(const char* type);
};
