#ifndef ENGINE_H
#define ENGINE_H


#include "window.h"
#include "event/e_window.h"
#include "../render/shader.h"
#include "../render/texture.h"
#include "../render/buffer.h"
#include <glm/glm.hpp>

namespace Core::Engine {

    struct EngineSpecs {
        std::string name = "EngineV1";
        std::string workingDirectory;
        std::filesystem::path iconPath;
        int32_t width = 1600, height = 900;
        bool decorated = true, fullscreen = false, maximized = false;
        bool darkMode = true, vSync = true, resizable = true, renderer = true;
    };

    class Engine {
        using EventCallbackFunc = std::function<void(Event&)>;

        private:
            std::unique_ptr<Window> window;
        struct RenderResources {
            std::unique_ptr<Render::Shader> shader;
            std::unique_ptr<Render::Buffer> quad;
            std::unique_ptr<Render::Texture> texture;
            glm::mat4 projection{1.0f};
            glm::mat4 view{1.0f};
        } renderResources;

        void initializeRenderResources();
        void cleanupRenderResources();
        void updateProjectionMatrix();
            
            // Camera and transformation data
            glm::mat4 projection;
            float rotationAngle = 0.0f;
            glm::vec3 position{0.0f};
            glm::vec3 scale{0.5f, 0.5f, 0.5f};
            float rotationSpeed = 50.0f;

            // Timing
            float deltaTime = 0.0f;
            float lastFrame = 0.0f;

            EngineSpecs specs;
            bool isTerminated = false;
            bool isMinimized = false;

            static Engine* instance;
            float timeScale = 1.0f;

            // Private helper functions
            void initializeGraphics();
            void processInput();
            void updateTransforms();
            void render();
            WindowSpecs createWindowSpecs() const;

        public:
            Engine(const EngineSpecs& specification);
            virtual ~Engine();

            virtual void OnInitialization() {}
            virtual void OnTermination() {}
            virtual void OnUpdate(float deltaTime) {}
            virtual void OnRender() {}
            virtual void OnEvent(Event& event) {}

            void Initialize();
            void Terminate();
            void Run();

            void HandleEvent(Event& event);

            // Getters and setters
            void SetTimeScale(float timeScale) { this->timeScale = timeScale; }
            float GetTimeScale() const { return timeScale; }
            float GetDeltaTime() const { return deltaTime; }

            // Transform controls
            void SetPosition(const glm::vec3& pos) { position = pos; }
            void SetScale(const glm::vec3& s) { scale = s; }
            void SetRotationSpeed(float speed) { rotationSpeed = speed; }
            void SetRotationAngle(float angle) { rotationAngle = angle; }

            inline Window& GetWindow() const { return *window; }
            static inline Engine& Get() { return *instance; }
            const EngineSpecs& GetSpecs() const { return specs; }
    };

    Engine* CreateEngine(int32_t argc, const uint8_t* argv[]);
}
#endif