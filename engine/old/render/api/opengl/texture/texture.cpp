//
// Created by Administrator on 5/20/2024.
//

//
// Created by Administrator on 5/19/2024.
//

#include "texture.h"
#include <glad/gl.h>
#include <stb_image.h>
#include <iostream>

namespace backend::renderapi::opengl {
    Texture::Texture(const std::string& path) {
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);
        // Set texture wrapping/filtering options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Failed to load texture: " << path << std::endl;
        }
        stbi_image_free(data);
    }

    void Texture::bind() const {
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    void Texture::unbind() const {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    Texture::~Texture() {
        glDeleteTextures(1, &ID);
    }
}
