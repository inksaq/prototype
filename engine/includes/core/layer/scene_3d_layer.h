//
// Created by admin on 12/01/2025.
//

#ifndef SCENE_3D_LAYER_H
#define SCENE_3D_LAYER_H

#include "layer.h"
#include "grid.h"
#include "../../render/camera.h"
#include <memory>

namespace Core::Engine {
    struct Scene3DResources {
        std::unique_ptr<Render::Shader> gridShader;
        std::unique_ptr<Render::Buffer> gridBuffer;
        glm::mat4 projection{1.0f};
        glm::mat4 view{1.0f};
        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
        float rotationAngle = 0.0f;
        float rotationSpeed = 0.0f;
    };

    class Scene3DLayer : public Layer {
    public:
        Scene3DLayer();
        ~Scene3DLayer() = default;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImGuiRender() override;

    private:
        void initializeRenderResources();
        void updateProjectionMatrix();
        void processInput(float deltaTime);
        void updateTransforms(float deltaTime);

        Scene3DResources renderResources;
        bool initialized = false;
    };
}

#endif //SCENE_3D_LAYER_H
