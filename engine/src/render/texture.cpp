#include "includes/render/texture.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace Core::Render {

Texture::Texture(const std::string& path, const TextureParameters& params)
    : textureID(0), width(0), height(0), channels(0), target(params.target) {

    // Load image data using stb_image
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);

    if (data) {
        initialize(data, params);
        stbi_image_free(data);
    } else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        throw std::runtime_error("Failed to load texture");
    }
}

Texture::Texture(const unsigned char* data, int width, int height, int channels,
                 const TextureParameters& params)
    : width(width), height(height), channels(channels), target(params.target) {
    initialize(data, params);
}

Texture::~Texture() {
    if (textureID != 0) {
        glDeleteTextures(1, &textureID);
    }
}

Texture::Texture(Texture&& other) noexcept
    : textureID(other.textureID), width(other.width),
      height(other.height), channels(other.channels), target(other.target) {
    other.textureID = 0;
}

Texture& Texture::operator=(Texture&& other) noexcept {
    if (this != &other) {
        if (textureID != 0) {
            glDeleteTextures(1, &textureID);
        }
        textureID = other.textureID;
        width = other.width;
        height = other.height;
        channels = other.channels;
        target = other.target;
        other.textureID = 0;
    }
    return *this;
}

void Texture::initialize(const unsigned char* data, const TextureParameters& params) {
    glGenTextures(1, &textureID);
    glBindTexture(target, textureID);

    setParameters(params);

    // Determine format based on number of channels
    GLenum format = GL_RGB;
    GLenum internalFormat = GL_RGB;
    if (channels == 1) {
        format = GL_RED;
        internalFormat = GL_RED;
    } else if (channels == 4) {
        format = GL_RGBA;
        internalFormat = GL_RGBA;
    }

    glTexImage2D(target, 0, internalFormat, width, height, 0, format,
                 GL_UNSIGNED_BYTE, data);

    if (params.generateMipmap) {
        glGenerateMipmap(target);
    }

    glBindTexture(target, 0);
}

void Texture::setParameters(const TextureParameters& params) {
    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, params.minFilter);
    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, params.magFilter);
    glTexParameteri(target, GL_TEXTURE_WRAP_S, params.wrapS);
    glTexParameteri(target, GL_TEXTURE_WRAP_T, params.wrapT);
}

void Texture::bind(unsigned int unit) const {
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(target, textureID);
}

void Texture::unbind() const {
    glBindTexture(target, 0);
}

} // namespace Core::Render