//
// Created by admin on 12/01/2025.
//

#ifndef SCENE_3D_LAYER_H
#define SCENE_3D_LAYER_H

#include "layer.h"
#include "grid.h"
#include <memory>

namespace Core::Engine {
    struct Scene3DResources {
        std::unique_ptr<Render::Shader> gridShader;
        std::unique_ptr<Render::VertexArray> gridVAO;
        std::shared_ptr<Render::VertexBuffer> gridBuffer;
        size_t gridVertexCount;

        // Camera properties
        glm::vec3 position{50.0f, 50.0f, 50.0f};  // Initial camera position
        glm::vec3 front{0.0f, 0.0f, -1.0f};       // Camera front vector
        glm::vec3 up{0.0f, 1.0f, 0.0f};           // Camera up vector
        glm::vec3 right;                          // Camera right vector (will be calculated)

        // Camera matrices
        glm::mat4 view{1.0f};
        glm::mat4 projection{1.0f};

        // Grid properties
        glm::vec3 scale;
        glm::vec4 gridColor;
        float fadeDistance;

        Scene3DResources() :
            gridVertexCount(0),
            position(0.0f),
            scale(1.0f),
            gridColor(1.0f, 1.0f, 1.0f, 1.0f),
            fadeDistance(500.0f) {}
    };

    struct GridSettings {
        bool show;
        float size;
        float spacing;
        glm::vec4 color;
        bool infinite;
        float fadeDistance;
    };

    class Scene3DLayer : public Layer {
    public:
        Scene3DLayer();
        ~Scene3DLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnEvent(Event& event) override;
        void OnImGuiRender() override;

    private:
        void initializeRenderResources();
        void updateProjectionMatrix();
        void processInput(float deltaTime);
        void updateTransforms(float deltaTime);
        void setGridProperties(const GridSettings& settings);

        float yaw = -90.0f;    // Initial yaw angle
        float pitch = 0.0f;    // Initial pitch angle
        float lastX = 800.0f;  // Initial mouse position
        float lastY = 600.0f;
        bool firstMouse = true;

        // Camera settings
        float mouseSensitivity = 0.1f;
        float moveSpeed = 5.0f;

        Scene3DResources renderResources;
        bool initialized = false;
    };
}

#endif //SCENE_3D_LAYER_H
