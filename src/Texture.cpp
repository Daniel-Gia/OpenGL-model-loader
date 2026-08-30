#include "Texture.h"

#include <cstring>
#include <stdexcept>

Texture::Texture(const char* image, const char* texType) {
    type = texType;
    slot = getSlotByType(texType);

    int widthImg;
    int heightImg;
    stbi_set_flip_vertically_on_load(0);
    unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, nullptr, 4);

    if (bytes == nullptr) {
        throw std::runtime_error(std::string("Failed to load texture: ") + image + " (" + stbi_failure_reason() + ")");
    }

    glGenTextures(1, &ID);
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);

    // Set the type of algorithm that is used to make the image smaller or bigger
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Set the way the texture repeats
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

    glGenerateMipmap(GL_TEXTURE_2D);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    // Cleans up
    stbi_image_free(bytes);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::setUniformHasTexture(Shader& shader, const char* uniform, bool hasTexture) {
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.activate();
    glUniform1i(texUni, hasTexture ? 1 : 0);
}

void Texture::setUniformSlot(Shader& shader, const char* uniform) const {
    GLuint texUni = glGetUniformLocation(shader.ID, uniform);
    shader.activate();
    glUniform1i(texUni, slot);
}

void Texture::bind() const {
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::deleteFromMemory() {
    glDeleteTextures(1, &ID);
}

GLuint Texture::getSlotByType(const char* type) {
    if (strcmp(type, "baseColor") == 0) {
        return 0;
    }
    if (strcmp(type, "metallicRoughness") == 0) {
        return 1;
    }

    throw std::runtime_error(std::string("Unknown texture type: ") + type);
}
