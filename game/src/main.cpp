#include "engine/includes/core/engine.h"
#include "engine/includes/engine.hpp"
#include <memory>
#include <iostream>

// Optional: Create a Game class that inherits from Engine to handle game-specific logic
class Game : public Core::Engine::Engine {
public:
    struct GridSettings {
        bool show = true;
        float size = 100.0f;
        float spacing = 1.0f;
        glm::vec4 color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f); // Gray, semi-transparent
        bool infinite = false;
        float fadeDistance = 500.0f;
    } gridSettings;

    explicit Game(const Core::Engine::EngineSpecs& specs) : Engine(specs) {
    }

    void RenderGrid() {
        if (!Core::Engine::GridSettings::show) return;

        // Get the debug layer
        auto debugLayer = GetLayerStack().GetLayerByName<Core::Engine::DebugLayer>("DebugLayer");
        if (!debugLayer) {
            std::cerr << "Debug layer not found!" << std::endl;
            return;
        }

        // Get the scene 3D layer
        auto scene3DLayer = GetLayerStack().GetLayerByName<Core::Engine::Scene3DLayer>("Scene3DLayer");
        if (!scene3DLayer) {
            std::cerr << "Scene3D layer not found!" << std::endl;
            return;
        }

        // Update grid settings from debug layer to scene layer
        scene3DLayer->setGridProperties({
            gridSettings.show,
            gridSettings.size,
            gridSettings.spacing,
            gridSettings.color,
            gridSettings.infinite,
            gridSettings.fadeDistance
        });
    }

    void OnInitialization() override {
        // Always call base class initialization first
        Engine::OnInitialization();
        std::cout << "Game::OnInitialization starting...\n";

        try {
            // Initialize layers
            auto scene3DLayer = std::make_shared<Core::Engine::Scene3DLayer>();
            auto imGuiLayer = std::make_shared<Core::Engine::ImGuiLayer>();
            auto debugLayer = std::make_shared<Core::Engine::DebugLayer>();

            std::cout << "Pushing Scene3DLayer...\n";
            PushLayer(scene3DLayer);

            std::cout << "Pushing ImGuiLayer...\n";
            PushOverlay(imGuiLayer);

            std::cout << "Pushing DebugLayer...\n";
            PushOverlay(debugLayer);

            gridSettings.show = true;
            gridSettings.size = 100.0f;
            gridSettings.spacing = 1.0f;
            gridSettings.color = glm::vec4(0.5f, 0.5f, 0.5f, 0.5f);
            gridSettings.infinite = false;
            gridSettings.fadeDistance = 500.0f;

        } catch (const std::exception& e) {
            std::cerr << "Error during layer initialization: " << e.what() << std::endl;
            throw; // Re-throw to handle in main
        }

        // Game-specific initialization
        SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
        SetRotationSpeed(0.0f);

        std::cout << "Game::OnInitialization completed.\n";
    }

    void OnUpdate(float deltaTime) override {
        // Always call base class update first
        Engine::OnUpdate(deltaTime);

        // Update and render the grid
        RenderGrid();

        // Debug: print active layers
        static float debugTimer = 0.0f;
        debugTimer += deltaTime;
        if (debugTimer >= 1.0f) {
            std::cout << "Active Layers:\n";
            for (const auto& layer : GetLayerStack()) {
                std::cout << "Layer: " << layer->GetName()
                         << " Enabled: " << layer->IsEnabled() << "\n";
            }
            debugTimer = 0.0f;
        }
    }

    void OnRender() override {
        // Always call base class render first
        Engine::OnRender();
    }

    void OnEvent(Core::Engine::Event& event) override {
        // Always call base class event handling first
        Engine::OnEvent(event);
    }
};

int main() {
    Core::Engine::EngineSpecs specs;
    specs.name = "My Game";
    specs.iconPath = "res/icon.png";
    specs.width = 1600;
    specs.height = 900;
    specs.vSync = true;
    specs.resizable = true;
    specs.renderer = false;
    specs.workingDirectory = std::filesystem::current_path().string();

    try {
        auto game = std::make_unique<Game>(specs);
        game->Initialize(); // This will now handle layer initialization
        game->Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}