//
// Created by Administrator on 12/29/2024.
//

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "buffer.h"
#include <memory>

namespace Core::Render {
    class VertexArray {
    public:
        virtual ~VertexArray() = default;

        virtual uint32_t GetID() const = 0;

        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) = 0;
        virtual void AddIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) = 0;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const = 0;
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const = 0;

        static std::shared_ptr<VertexArray> Create();
    };
}

#endif //VERTEX_ARRAY_H
