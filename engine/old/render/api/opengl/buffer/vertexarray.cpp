//
// Created by Administrator on 5/20/2024.
//

#include "vertexarray.h"

#include "vertexarray.h"

namespace backend::renderapi::opengl {
    VertexArray::VertexArray() {
        glGenVertexArrays(1, &ID);
    }

    void VertexArray::bind() const {
        glBindVertexArray(ID);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &ID);
    }

    void VertexArray::setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, size, type, normalized, stride, pointer);
    }
}
