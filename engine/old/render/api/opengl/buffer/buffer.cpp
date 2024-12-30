//
// Created by Administrator on 5/17/2024.
//

#include <glad/gl.h>
#include "buffer.h"


namespace backend::renderapi::opengl {
    Buffer::Buffer(const std::vector<float>& data, GLenum usage, GLenum target) : target(target) {
        glGenBuffers(1, &ID);
        glBindBuffer(target, ID);
        glBufferData(target, data.size() * sizeof(float), data.data(), usage);
        glBindBuffer(target, 0);
    }

    Buffer::Buffer(const std::vector<uint32_t>& data, GLenum usage, GLenum target) : target(target) {
        glGenBuffers(1, &ID);
        glBindBuffer(target, ID);
        glBufferData(target, data.size() * sizeof(uint32_t), data.data(), usage);
        glBindBuffer(target, 0);
    }

    void Buffer::bind() const {
        glBindBuffer(target, ID);
    }

    void Buffer::unbind() const {
        glBindBuffer(target, 0);
    }

    Buffer::~Buffer() {
        glDeleteBuffers(1, &ID);
    }
}
