#ifndef ENGINE_BUFFER_H
#define ENGINE_BUFFER_H

#include "vertex_array.h"

#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <initializer_list>
#include <sys/stat.h>

namespace Core::Render {

    enum class ShaderDataType {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type) {
        switch (type) {
            case ShaderDataType::Float:    return 4;
            case ShaderDataType::Float2:   return 4 * 2;
            case ShaderDataType::Float3:   return 4 * 3;
            case ShaderDataType::Float4:   return 4 * 4;
            case ShaderDataType::Mat3:     return 4 * 3 * 3;
            case ShaderDataType::Mat4:     return 4 * 4 * 4;
            case ShaderDataType::Int:      return 4;
            case ShaderDataType::Int2:     return 4 * 2;
            case ShaderDataType::Int3:     return 4 * 3;
            case ShaderDataType::Int4:     return 4 * 4;
            case ShaderDataType::Bool:     return 1;
            case ShaderDataType::None:     return 0;
        }
        return 0;
    }

    struct BufferElement {
        std::string name;
        ShaderDataType type;
        uint32_t size;
        uint32_t offset;
        bool normalized;

        BufferElement(ShaderDataType type_, const std::string& name_, bool normalized_ = false)
            : name(name_), type(type_), size(ShaderDataTypeSize(type_)), offset(0), normalized(normalized_) {}

        uint32_t getComponentCount() const {
            switch (type) {
                case ShaderDataType::Float:   return 1;
                case ShaderDataType::Float2:  return 2;
                case ShaderDataType::Float3:  return 3;
                case ShaderDataType::Float4:  return 4;
                case ShaderDataType::Mat3:    return 3 * 3;
                case ShaderDataType::Mat4:    return 4 * 4;
                case ShaderDataType::Int:     return 1;
                case ShaderDataType::Int2:    return 2;
                case ShaderDataType::Int3:    return 3;
                case ShaderDataType::Int4:    return 4;
                case ShaderDataType::Bool:    return 1;
                case ShaderDataType::None:    return 0;
            }
            return 0;
        }
    };

    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferElement>& elements)
            : m_Elements(elements) {
            calculateOffsetsAndStride();
        }

        uint32_t getStride() const { return m_Stride; }
        const std::vector<BufferElement>& getElements() const { return m_Elements; }
        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        void calculateOffsetsAndStride() {
            uint32_t offset = 0;
            m_Stride = 0;
            for (auto& element : m_Elements) {
                element.offset = offset;
                offset += element.size;
                m_Stride += element.size;
            }
        }

        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class Buffer {
    public:
        static std::shared_ptr<class VertexBuffer> createVertexBuffer(float* vertices, uint32_t size);
        static std::shared_ptr<class VertexBuffer> createDynamicVertexBuffer(uint32_t size);
        static std::shared_ptr<class IndexBuffer> createIndexBuffer(uint32_t* indices, uint32_t count);
        static std::shared_ptr<class VertexArray> createQuad();

        static void copyBufferData(uint32_t srcBuffer, uint32_t dstBuffer, uint32_t size);
        static bool checkGLError(const char* operation);    };

    class VertexBuffer {
    public:
        VertexBuffer(float* vertices, uint32_t size);
        VertexBuffer(uint32_t size); // For dynamic buffers
        ~VertexBuffer();

        // Prevent copying
        VertexBuffer(const VertexBuffer&) = delete;
        VertexBuffer& operator=(const VertexBuffer&) = delete;

        // Allow moving
        VertexBuffer(VertexBuffer&& other) noexcept;
        VertexBuffer& operator=(VertexBuffer&& other) noexcept;

        void bind() const;
        void unbind() const;

        void setData(const void* data, uint32_t size);
        void setLayout(const BufferLayout& layout) { m_Layout = layout; }
        const BufferLayout& getLayout() const { return m_Layout; }

        uint32_t getID() const { return m_RendererID; }
        uint32_t getSize() const { return m_Size; }
        bool isDynamic() const { return m_Dynamic; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Size;
        bool m_Dynamic;
        BufferLayout m_Layout;
    };

    class IndexBuffer {
    public:
        IndexBuffer(uint32_t* indices, uint32_t count);
        ~IndexBuffer();

        // Prevent copying
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer& operator=(const IndexBuffer&) = delete;

        // Allow moving
        IndexBuffer(IndexBuffer&& other) noexcept;
        IndexBuffer& operator=(IndexBuffer&& other) noexcept;

        void bind() const;
        void unbind() const;

        uint32_t getCount() const { return m_Count; }
        uint32_t getID() const { return m_RendererID; }

    private:
        uint32_t m_RendererID;
        uint32_t m_Count;
    };



} // namespace Core::Render

#endif // ENGINE_BUFFER_H