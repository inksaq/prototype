//
// Created by admin on 11/01/2025.
//

#ifndef GRID_H
#define GRID_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "../../render/vertex_array.h"
#include "../../render/shader.h"

namespace Core::Engine {
    class Grid {
    public:
        Grid(float size = 100.0f, float spacing = 1.0f);
        ~Grid() = default;

        void Draw(const glm::mat4& viewProjection);
        void SetColor(const glm::vec4& color) { gridColor = color; }
        void SetFadeDistance(float distance) { fadeDistance = distance; }

    private:
        void GenerateGrid();

        std::unique_ptr<Render::VertexArray> vao;
        std::unique_ptr<Render::VertexBuffer> vbo;
        std::unique_ptr<Render::Shader> shader;

        float size;
        float spacing;
        glm::vec4 gridColor{0.5f, 0.5f, 0.5f, 0.5f};
        float fadeDistance = 100.0f;
        std::vector<float> vertices;
    };
}

#endif //GRID_H
