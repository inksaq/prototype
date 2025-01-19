//
// Created by admin on 11/01/2025.
//

#ifndef DEBUG_LAYER_H
#define DEBUG_LAYER_H

#include "layer.h"
#include <imgui.h>
#include "../window.h"

namespace Core::Engine {

    class DebugLayer : public Layer {
    public:
        DebugLayer();
        ~DebugLayer() override;

        void OnAttach() override;
        void OnDetach() override;
        void OnUpdate(float deltaTime) override;
        void OnRender() override;
        void OnImguiRender();
        void OnEvent(Event& event) override;

        void Begin();
        void End();


    private:

        float time = 0.0f;
    };
}

#endif //DEBBUG_LAYER_H
