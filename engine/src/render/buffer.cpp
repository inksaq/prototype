#include <GL/glew.h>
#include <iostream>
#include <render/buffer.h>

namespace Core::Render {

    ////////////////////////////////////////////////////////////////////////////
    // VertexBuffer /////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    VertexBuffer::VertexBuffer(float* vertices, uint32_t size) {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

        // Debug verification
        float* debugData = new float[size/sizeof(float)];
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, debugData);
        std::cout << "Vertex Buffer Data Verification:" << std::endl;
        for(size_t i = 0; i < size/sizeof(float); i++) {
            std::cout << "Vertex[" << i << "]: " << debugData[i];
            if(i % 5 == 4) std::cout << " (end of vertex)";  // Since each vertex has 5 components
            std::cout << std::endl;
        }
        delete[] debugData;
    }

    VertexBuffer::VertexBuffer(uint32_t size) {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept
        : m_RendererID(other.m_RendererID), m_Layout(std::move(other.m_Layout)) {
        other.m_RendererID = 0;
    }

    VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
        if (this != &other) {
            glDeleteBuffers(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            m_Layout = std::move(other.m_Layout);
            other.m_RendererID = 0;
        }
        return *this;
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::setData(const void* data, uint32_t size) {
        glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
        // Add debug check
        float* debugData = new float[size/sizeof(float)];
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, size, debugData);
        for(size_t i = 0; i < size/sizeof(float); i++) {
            std::cout << "Vertex data[" << i << "]: " << debugData[i] << std::endl;
        }
        delete[] debugData;
    }


    ////////////////////////////////////////////////////////////////////////////
    // IndexBuffer //////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count)
        : m_Count(count) {
        glCreateBuffers(1, &m_RendererID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_RendererID);
    }

    IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept
        : m_RendererID(other.m_RendererID), m_Count(other.m_Count) {
        other.m_RendererID = 0;
        other.m_Count = 0;
    }

    IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
        if (this != &other) {
            glDeleteBuffers(1, &m_RendererID);
            m_RendererID = other.m_RendererID;
            m_Count = other.m_Count;
            other.m_RendererID = 0;
            other.m_Count = 0;
        }
        return *this;
    }

    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    }

    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ////////////////////////////////////////////////////////////////////////////
    // Buffer /////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////

    std::shared_ptr<VertexArray> Buffer::createQuad() {
        auto vertexArray = std::make_shared<VertexArray>();

        // Vertex data with debug output
        float vertices[] = {
            // positions        // texture coords
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left
        };

        std::cout << "Creating quad with vertices:" << std::endl;
        for(int i = 0; i < 4; i++) { // 4 vertices
            std::cout << "Vertex " << i << ": "
                      << "pos(" << vertices[i*5] << ", " << vertices[i*5+1] << ", " << vertices[i*5+2] << ") "
                      << "tex(" << vertices[i*5+3] << ", " << vertices[i*5+4] << ")" << std::endl;
        }

        auto vertexBuffer = std::make_shared<VertexBuffer>(vertices, sizeof(vertices));

        // Set up layout with debug info
        BufferLayout layout = {
            { ShaderDataType::Float3, "a_Position" },
            { ShaderDataType::Float2, "a_TexCoord" }
        };

        std::cout << "Buffer layout:" << std::endl;
        std::cout << "Stride: " << layout.getStride() << " bytes" << std::endl;
        for(const auto& element : layout) {
            std::cout << "Attribute: " << element.name
                      << " Offset: " << element.offset
                      << " Size: " << element.size << std::endl;
        }

        vertexBuffer->setLayout(layout);
        vertexArray->addVertexBuffer(vertexBuffer);

        uint32_t indices[] = {
            0, 1, 2,    // first triangle
            2, 3, 0     // second triangle
        };

        std::cout << "Indices: ";
        for(int i = 0; i < 6; i++) {
            std::cout << indices[i] << " ";
        }
        std::cout << std::endl;

        auto indexBuffer = std::make_shared<IndexBuffer>(indices, sizeof(indices) / sizeof(uint32_t));
        vertexArray->setIndexBuffer(indexBuffer);

        return vertexArray;
    }

} // namespace Core::Render