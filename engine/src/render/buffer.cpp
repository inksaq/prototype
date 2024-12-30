//
// Created by Administrator on 12/29/2024.
//

#include <includes/render/buffer.h>
#include <iostream>

namespace Core::Render {

Buffer::Buffer() : vao(0), vbo(0), ebo(0), vertexCount(0), indexCount(0) {
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
}

Buffer::~Buffer() {
    cleanup();
}

Buffer::Buffer(Buffer&& other) noexcept
    : vao(other.vao), vbo(other.vbo), ebo(other.ebo),
      vertexCount(other.vertexCount), indexCount(other.indexCount) {
    other.vao = other.vbo = other.ebo = 0;
    other.vertexCount = other.indexCount = 0;
}

Buffer& Buffer::operator=(Buffer&& other) noexcept {
    if (this != &other) {
        cleanup();
        vao = other.vao;
        vbo = other.vbo;
        ebo = other.ebo;
        vertexCount = other.vertexCount;
        indexCount = other.indexCount;
        other.vao = other.vbo = other.ebo = 0;
        other.vertexCount = other.indexCount = 0;
    }
    return *this;
}

void Buffer::bind() const {
    glBindVertexArray(vao);
}

void Buffer::unbind() const {
    glBindVertexArray(0);
}

void Buffer::setVertexData(const std::vector<Vertex>& vertices) {
    bind();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
    setupVertexAttributes();
    vertexCount = vertices.size();
}

void Buffer::setIndices(const std::vector<uint32_t>& indices) {
    bind();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_STATIC_DRAW);
    indexCount = indices.size();
}

void Buffer::setupVertexAttributes() {
    // Position attribute (location = 0)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

    // Texture coordinates attribute (location = 1)
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
}

void Buffer::draw() const {
    bind();
    glDrawArrays(GL_TRIANGLES, 0, vertexCount);
}

void Buffer::drawIndexed() const {
    bind();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void Buffer::cleanup() {
    if (vao) glDeleteVertexArrays(1, &vao);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (ebo) glDeleteBuffers(1, &ebo);
    vao = vbo = ebo = 0;
}

// Static create functions
std::unique_ptr<Buffer> Buffer::createQuad() {
    auto buffer = std::make_unique<Buffer>();

    std::vector<Vertex> vertices = {
        // Vertices for a quad centered at origin
        {{-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f}}, // Top left
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, // Bottom left
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}, // Bottom right
        {{ 0.5f,  0.5f, 0.0f}, {1.0f, 1.0f}}  // Top right
    };

    std::vector<uint32_t> indices = {
        0, 1, 2,  // First triangle
        0, 2, 3   // Second triangle
    };

    buffer->setVertexData(vertices);
    buffer->setIndices(indices);

    return buffer;
}

std::unique_ptr<Buffer> Buffer::createTriangle() {
    auto buffer = std::make_unique<Buffer>();

    std::vector<Vertex> vertices = {
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}}, // Bottom left
        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}}, // Bottom right
        {{ 0.0f,  0.5f, 0.0f}, {0.5f, 1.0f}}  // Top
    };

    buffer->setVertexData(vertices);
    return buffer;
}

} // namespace Core::Render