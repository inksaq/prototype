//
// Created by admin on 11/01/2025.
//

#include "core/layer/imgui_layer.h"
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include "core/engine.h"
namespace Core::Engine {
    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer") {}

    ImGuiLayer::~ImGuiLayer() {}

    void ImGuiLayer::OnAttach() {
        // Initialize ImGui
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // Setup style
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer bindings
        GLFWwindow* window = static_cast<GLFWwindow*>(Engine::Get().GetWindow().getNativeWindow());
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach() {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::OnUpdate(float deltaTime) {
    }

    void ImGuiLayer::OnEvent(Event& event) {

    }


    void ImGuiLayer::End() {
        ImGuiIO& io = ImGui::GetIO();
        Engine& engine = Engine::Get();
        io.DisplaySize = ImVec2((float)engine.GetWindow().getSize().x, (float)engine.GetWindow().getSize().y);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags /*& ImGuiConfigFlags_ViewportsEnable*/) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            /*ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();*/
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    void ImGuiLayer::OnImGuiRender() {
        static bool show = true;
        ImGui::ShowDemoWindow(&show);
    }
}
