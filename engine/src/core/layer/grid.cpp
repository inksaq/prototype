//
// Created by admin on 11/01/2025.
//

#include "core/layer/grid.h"
#include <GL/glew.h>

namespace Core::Engine {
    Grid::Grid(float size, float spacing)
        : size(size), spacing(spacing) {

        shader = std::make_unique<Render::Shader>(
            "res/shaders/grid.vert", "res/shaders/grid.frag");
        vao = std::make_unique<Render::VertexArray>();

        GenerateGrid();

        vbo = std::make_unique<Render::VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));

        Render::BufferLayout layout = {
            { Render::ShaderDataType::Float3, "aPos" }
        };

        vbo->setLayout(layout);
        vao->addVertexBuffer(std::shared_ptr<Render::VertexBuffer>(vbo.get()));
    }

    void Grid::GenerateGrid() {
        vertices.clear();

        // Generate horizontal lines
        for (float i = -size; i <= size; i += spacing) {
            vertices.push_back(-size); // x
            vertices.push_back(0.0f);  // y
            vertices.push_back(i);     // z

            vertices.push_back(size);  // x
            vertices.push_back(0.0f);  // y
            vertices.push_back(i);     // z
        }

        // Generate vertical lines
        for (float i = -size; i <= size; i += spacing) {
            vertices.push_back(i);     // x
            vertices.push_back(0.0f);  // y
            vertices.push_back(-size); // z

            vertices.push_back(i);     // x
            vertices.push_back(0.0f);  // y
            vertices.push_back(size);  // z
        }
    }

    void Grid::Draw(const glm::mat4& viewProjection) {
        shader->use();
        shader->setMat4("uViewProjection", viewProjection);
        shader->setVec4("uGridColor", gridColor);
        shader->setFloat("uFadeDistance", fadeDistance);

        vao->bind();
        glDrawArrays(GL_LINES, 0, vertices.size() / 3);
        vao->unbind();
    }
}