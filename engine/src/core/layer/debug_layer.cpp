//
// Created by admin on 11/01/2025.
//



#include <imgui.h>
#include <iostream>
#include <core/layer/debug_layer.h>

#include "core/layer/layer.h"
#include "core/engine.h"
#include <glm/gtc/type_ptr.hpp>

namespace Core::Engine {

        DebugLayer::DebugLayer() : Layer("DebugLayer") {}

        DebugLayer::~DebugLayer() = default;

        void DebugLayer::OnAttach() {
            Layer::OnAttach();
        }

        void DebugLayer::OnDetach() {
            Layer::OnDetach();
        }

        void DebugLayer::OnUpdate(float deltaTime) {
            Layer::OnUpdate(deltaTime);
        }

        void DebugLayer::OnEvent(Event &event) {
            Layer::OnEvent(event);
        }

        void DebugLayer::Begin() {
        }

        void DebugLayer::End() {
        }


        void DebugLayer::OnImguiRender() {
            ImGui::Begin("Debug Information");

            if (ImGui::CollapsingHeader("Performance", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::Text("Frame Time: %.3f ms", ImGui::GetIO().DeltaTime * 1000.0f);
            }

            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen)) {
                auto& camera = Engine::Get().GetCamera();

                // Position
                glm::vec3 position = camera.Position;
                if (ImGui::DragFloat3("Position", glm::value_ptr(position), 0.1f)) {
                    camera.Position = position;
                }

                // Rotation
                float yaw = camera.Yaw;
                float pitch = camera.Pitch;
                if (ImGui::DragFloat("Yaw", &yaw, 0.5f)) {
                    camera.Yaw = yaw;
                }
                if (ImGui::DragFloat("Pitch", &pitch, 0.5f, -89.0f, 89.0f)) {
                    camera.Pitch = pitch;
                }

                // Camera Settings
                float moveSpeed = camera.MovementSpeed;
                if (ImGui::DragFloat("Movement Speed", &moveSpeed, 0.1f, 0.1f, 20.0f)) {
                    camera.MovementSpeed = moveSpeed;
                }

                float sensitivity = camera.MouseSensitivity;
                if (ImGui::DragFloat("Mouse Sensitivity", &sensitivity, 0.01f, 0.01f, 1.0f)) {
                    camera.MouseSensitivity = sensitivity;
                }

                float zoom = camera.Zoom;
                if (ImGui::DragFloat("FOV", &zoom, 0.5f, 1.0f, 45.0f)) {
                    camera.Zoom = zoom;
                }

                if (ImGui::Button("Reset Camera")) {
                    camera.Position = glm::vec3(0.0f, 0.0f, 3.0f);
                    camera.Yaw = -90.0f;
                    camera.Pitch = 0.0f;
                }
            }

            if (ImGui::CollapsingHeader("Grid Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Checkbox("Show Grid", &gridSettings.show);
                ImGui::DragFloat("Grid Size", &gridSettings.size, 1.0f, 1.0f, 1000.0f);
                ImGui::DragFloat("Grid Spacing", &gridSettings.spacing, 0.1f, 0.1f, 10.0f);
                ImGui::ColorEdit4("Grid Color", &gridSettings.color.x);
                ImGui::Checkbox("Infinite Grid", &gridSettings.infinite);
            }

            if (ImGui::CollapsingHeader("Controls Info", ImGuiTreeNodeFlags_DefaultOpen)) {
                ImGui::Text("Camera Controls:");
                ImGui::BulletText("WASD - Move camera");
                ImGui::BulletText("Q/E - Move up/down");
                ImGui::BulletText("Right Mouse - Look around");
                ImGui::BulletText("Mouse Wheel - Zoom");
                ImGui::BulletText("R - Reset camera");
            }

            ImGui::End();
        }

        void DebugLayer::OnRender() {
            if (gridSettings.show) {
            std::cout << "Rendering Grid" << std::endl;
            }
        }


}