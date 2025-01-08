#include "engine/includes/core/engine.h"
#include "engine/includes/engine.hpp"
#include <memory>
#include <iostream>

// Optional: Create a Game class that inherits from Engine to handle game-specific logic
class Game : public Core::Engine::Engine {
public:
    explicit Game(const Core::Engine::EngineSpecs& specs) : Engine(specs) {}

    void OnInitialization() override {
        // Always call base class initialization first
        Engine::OnInitialization();
        std::cout << "init\n";

        // Your game initialization code here
        SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
        SetRotationSpeed(0.0f);  // Set to non-zero for rotation

    }

    void OnUpdate(float deltaTime) override {
        // Always call base class update first
        Engine::OnUpdate(deltaTime);

    }

    void OnRender() override {
        // Always call base class render first
        Engine::OnRender();

        // Your additional rendering code here
    }

    void OnEvent(Core::Engine::Event& event) override {
        // Always call base class event handling first
        Engine::OnEvent(event);

        // Your event handling code here
    }

};

int main() {
    // Configure engine specifications
    Core::Engine::EngineSpecs specs;
    specs.name = "My Game";
    specs.iconPath = "res/icon.png";
    specs.width = 1600;
    specs.height = 900;
    specs.vSync = true;
    specs.resizable = true;
    specs.renderer = true;
    specs.workingDirectory = std::filesystem::current_path().string();


    // Create and run the game
    try {
        auto game = std::make_unique<Game>(specs);
        game->Initialize();
        game->Run();


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}