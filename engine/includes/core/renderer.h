//
// Created by Administrator on 12/30/2024.
//

#ifndef RENDERER_H
#define RENDERER_H

#include <engine/includes/render/shader.h>

namespace Core::Engine {

    class Shader;

    class Renderer {
    public:
        static void Initialize();
        static void Terminate();

        static void OnWindowResize(uint32_t width, uint32_t height);

        static void BeginScene();
        static void EndScene();

        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
    };
}

}


#endif //RENDERER_H
