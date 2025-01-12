//
// Created by admin on 11/01/2025.
//

#pragma once

#include <imgui.h>
#include "core/layer/layer.h"

namespace Core::Engine {

    class DebugLayer : public Layer {
    public:
        DebugLayer() : Layer("DebugLayer") {}

        void OnImGuiRender() override {
            ImGui::Begin("Debug Information");

            if (ImGui::CollapsingHeader("Performance")) {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::Text("Frame Time: %.3f ms", ImGui::GetIO().DeltaTime * 1000.0f);
            }

            if (ImGui::CollapsingHeader("Camera")) {
                // Add camera debug info here
            }

            if (ImGui::CollapsingHeader("Layers")) {
                // List all active layers
            }

            ImGui::End();
        }
    };
}