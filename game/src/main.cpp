#include "engine/includes/core/engine.h"
#include <memory>
#include <iostream>

// Optional: Create a Game class that inherits from Engine to handle game-specific logic
class Game : public Core::Engine::Engine {
public:
    explicit Game(const Core::Engine::EngineSpecs& specs) : Engine(specs) {}

    // Override engine callbacks for game-specific behavior
    void OnInitialization() override {

    }

    void OnUpdate(float deltaTime) override {
        // Game-specific update logic
        // Note: Basic input handling is already in the engine
    }

    void OnRender() override {
        // Any additional rendering
        // Note: Basic sprite rendering is already handled in the engine
    }

    void OnEvent(Core::Engine::Event& event) override {
        // Handle any game-specific events
    }
};

int main() {
    // Configure engine specifications
    Core::Engine::EngineSpecs specs;
    specs.name = "My Game";        // Window title
    specs.width = 1600;
    specs.height = 900;
    specs.vSync = true;
    specs.resizable = true;
    
    // Create and run the game
    try {
        Game game(specs);
        game.Initialize();
        game.Run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }

    return 0;
}