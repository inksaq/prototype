#ifndef ENGINE_BUFFER_H
#define ENGINE_BUFFER_H

#include <GL/glew.h>
#include <vector>
#include <memory>
#include <glm/glm.hpp>

namespace Core::Render {

    // Vertex structure for standard 2D/3D rendering
    struct Vertex {
        glm::vec3 position;
        glm::vec2 texCoords;
    };

    class Buffer {
    public:
        // Constructor and destructor
        Buffer();
        ~Buffer();

        // No copying allowed
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;

        // Move operations
        Buffer(Buffer&& other) noexcept;
        Buffer& operator=(Buffer&& other) noexcept;

        // Core functionality
        void bind() const;
        void unbind() const;

        // Vertex data functions
        void setVertexData(const std::vector<Vertex>& vertices);
        void setIndices(const std::vector<uint32_t>& indices);

        // Drawing functions
        void draw() const;
        void drawIndexed() const;

        // Static create functions for common shapes
        static std::unique_ptr<Buffer> createQuad();
        static std::unique_ptr<Buffer> createTriangle();

    private:
        GLuint vao;
        GLuint vbo;
        GLuint ebo;
        size_t vertexCount;
        size_t indexCount;

        void setupVertexAttributes();
        void cleanup();
    };

} // namespace Core::Render

#endif // ENGINE_BUFFER_H