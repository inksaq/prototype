//
// Created by admin on 11/01/2025.
//

#ifndef IMGUI_LAYER_H
#define IMGUI_LAYER_H

#include "layer.h"
#include <imgui.h>
#include "../window.h"

namespace Core::Engine {
    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnImGuiRender() override;
        void OnEvent(Event& event) override;

        void Begin();
        void End();

    private:
        float time = 0.0f;
        bool show_demo_window = true;
    };
}

#endif //IMGUI_LAYER_H
