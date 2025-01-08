//
// Created by Administrator on 12/29/2024.
//


#include <render/buffer.h>
#include <GL/glew.h>
#include <iostream>

namespace Core::Render {

    static GLenum shaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:    return GL_FLOAT;
            case ShaderDataType::Float2:   return GL_FLOAT;
            case ShaderDataType::Float3:   return GL_FLOAT;
            case ShaderDataType::Float4:   return GL_FLOAT;
            case ShaderDataType::Mat3:     return GL_FLOAT;
            case ShaderDataType::Mat4:     return GL_FLOAT;
            case ShaderDataType::Int:      return GL_INT;
            case ShaderDataType::Int2:     return GL_INT;
            case ShaderDataType::Int3:     return GL_INT;
            case ShaderDataType::Int4:     return GL_INT;
            case ShaderDataType::Bool:     return GL_BOOL;
        }

        std::cerr << "Unknown ShaderDataType!" << std::endl;
        return 0;
    }

    VertexArray::VertexArray()
        : m_RendererID(0) {
        glCreateVertexArrays(1, &m_RendererID);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_RendererID);
    }

    VertexArray::VertexArray(VertexArray&& other) noexcept
        : m_RendererID(other.m_RendererID),
          m_VertexBufferIndex(other.m_VertexBufferIndex),
          m_IndexBuffer(std::move(other.m_IndexBuffer)
          ) {
        other.m_RendererID = 0;
        other.m_VertexBufferIndex = 0;
    }

    VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
        if (this != &other) {
            glDeleteVertexArrays(1, &m_RendererID);

            m_RendererID = other.m_RendererID;
            m_VertexBufferIndex = other.m_VertexBufferIndex;
            m_IndexBuffer = std::move(other.m_IndexBuffer);
            m_VertexBuffer = std::move(other.m_VertexBuffer);

            other.m_RendererID = 0;
            other.m_VertexBufferIndex = 0;
        }
        return *this;
    }

    void VertexArray::bind() const {
        glBindVertexArray(m_RendererID);
        if (m_VertexBuffer) {
            m_VertexBuffer->bind();  // Ensure VBO is bound
        }
        if (m_IndexBuffer) {
            m_IndexBuffer->bind();   // Ensure IBO is bound
        }
        // Debug vertex array state
        GLint vao;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        std::cout << "Current VAO: " << vao << std::endl;

        // Debug attribute enables
        GLint maxAttribs;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttribs);
        for(int i = 0; i < maxAttribs; i++) {
            GLint enabled;
            glGetVertexAttribiv(i, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
            if(enabled) {
                std::cout << "Attribute " << i << " is enabled" << std::endl;
            }
        }
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::addVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) {
        m_VertexBuffer = vertexBuffer;  // Store the vertex buffer
        glBindVertexArray(m_RendererID);
        vertexBuffer->bind();

        const auto& layout = vertexBuffer->getLayout();
        std::cout << "Setting up vertex attributes:" << std::endl;
        for (const auto& element : layout) {
            std::cout << "Attribute: " << element.name
                      << " Components: " << element.getComponentCount()
                      << " Offset: " << element.offset
                      << " Stride: " << layout.getStride() << std::endl;
            switch (element.type) {
                case ShaderDataType::Float:
                case ShaderDataType::Float2:
                case ShaderDataType::Float3:
                case ShaderDataType::Float4: {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(m_VertexBufferIndex,
                                        element.getComponentCount(),
                                        shaderDataTypeToOpenGLBaseType(element.type),
                                        element.normalized ? GL_TRUE : GL_FALSE,
                                        layout.getStride(),
                                        (const void*)element.offset);
                    // Verify attribute state
                    GLint enabled;
                    glGetVertexAttribiv(m_VertexBufferIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
                    GLint size;
                    glGetVertexAttribiv(m_VertexBufferIndex, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
                    GLint stride;
                    glGetVertexAttribiv(m_VertexBufferIndex, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);

                    std::cout << "Attribute " << m_VertexBufferIndex
                              << " Enabled: " << enabled
                              << " Size: " << size
                              << " Stride: " << stride << std::endl;

                    m_VertexBufferIndex++;
                    break;
                    break;
                }
                case ShaderDataType::Int:
                case ShaderDataType::Int2:
                case ShaderDataType::Int3:
                case ShaderDataType::Int4:
                case ShaderDataType::Bool: {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribIPointer(m_VertexBufferIndex,
                                         element.getComponentCount(),
                                         shaderDataTypeToOpenGLBaseType(element.type),
                                         layout.getStride(),
                                         (const void*)element.offset);
                    m_VertexBufferIndex++;
                    break;
                }
                case ShaderDataType::Mat3:
                case ShaderDataType::Mat4: {
                    uint8_t count = element.getComponentCount();
                    for (uint8_t i = 0; i < count; i++) {
                        glEnableVertexAttribArray(m_VertexBufferIndex);
                        glVertexAttribPointer(m_VertexBufferIndex,
                                            count,
                                            shaderDataTypeToOpenGLBaseType(element.type),
                                            element.normalized ? GL_TRUE : GL_FALSE,
                                            layout.getStride(),
                                            (const void*)(element.offset + sizeof(float) * count * i));
                        glVertexAttribDivisor(m_VertexBufferIndex, 1);
                        m_VertexBufferIndex++;
                    }
                    break;
                }
                default:
                    std::cerr << "Unknown ShaderDataType!" << std::endl;
            }
        }
    }

    void VertexArray::setIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) {
        glBindVertexArray(m_RendererID);
        indexBuffer->bind();
        m_IndexBuffer = indexBuffer;
    }

} // namespace Core::Render