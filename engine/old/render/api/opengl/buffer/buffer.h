//
// Created by Administrator on 5/17/2024.
//

#ifndef BUFFER_H
#define BUFFER_H

#include <vector>
#include <glad/gl.h>

namespace backend::renderapi::opengl {
    class Buffer {
    public:
        Buffer(const std::vector<float>& data, GLenum usage, GLenum target = GL_ARRAY_BUFFER);
        Buffer(const std::vector<uint32_t>& data, GLenum usage, GLenum target = GL_ELEMENT_ARRAY_BUFFER);
        void bind() const;
        void unbind() const;
        ~Buffer();

    private:
        GLuint ID;
        GLenum target;
    };
}

#endif // BUFFER_H

