//
// Created by Administrator on 5/20/2024.
//
#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <glad/gl.h>

namespace backend::renderapi::opengl {
    class VertexArray {
    public:
        VertexArray();
        void bind() const;
        void unbind() const;
        ~VertexArray();

        void setAttribPointer(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);

    private:
        GLuint ID;
    };
}

#endif // VERTEXARRAY_H

