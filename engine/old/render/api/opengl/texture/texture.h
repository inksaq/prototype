//
// Created by Administrator on 5/19/2024.
//

#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/gl.h>

namespace backend::renderapi::opengl {
    class Texture {
    public:
        Texture(const std::string& path);
        void bind() const;
        void unbind() const;
        ~Texture();

    private:
        GLuint ID;
    };
}

#endif // TEXTURE_H
