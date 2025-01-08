//
// Created by Administrator on 12/29/2024.
//

#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "buffer.h"
#include <memory>

namespace Core::Render {
    class IndexBuffer;
    class VertexBuffer;

    class VertexArray {
    public:
        VertexArray();
        ~VertexArray();

        // Prevent copying
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;

        // Allow moving
        VertexArray(VertexArray&& other) noexcept;
        VertexArray& operator=(VertexArray&& other) noexcept;

        void bind() const;
        void unbind() const;

        // Add vertex buffer with layout
        void addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

        // Set index buffer
        void setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

        // Getters
        const std::shared_ptr<IndexBuffer>& getIndexBuffer() const { return m_IndexBuffer; }
        uint32_t getVertexArrayID() const { return m_RendererID; }

    private:
        uint32_t m_RendererID;
        uint32_t m_VertexBufferIndex = 0;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;  // Add this line
    };
}

#endif //VERTEX_ARRAY_H
