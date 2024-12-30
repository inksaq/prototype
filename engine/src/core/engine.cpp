#include "core/engine.h"
#include "core/input.h"
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <shaders/default_shaders.hh>

namespace Core::Engine {
    WindowSpecs Engine::createWindowSpecs() const {
        return WindowSpecs{
            .title = specs.name,
            .iconPath = specs.iconPath,
            .width = specs.width,
            .height = specs.height,
            .decorated = specs.decorated,
            .fullscreen = specs.fullscreen,
            .maximised = specs.maximized,
            .darkMode = specs.darkMode,
            .vSync = specs.vSync,
            .resizable = specs.resizable
        };
    }
    Engine* Engine::instance = nullptr;

    Engine::Engine(const EngineSpecs& specification) : specs(specification) {
        instance = this;

        if (!specification.workingDirectory.empty())
            std::filesystem::current_path(specification.workingDirectory);

        std::log("test")// TODO ADD LOGGER
        window = std::unique_ptr<Window>(Window::create(createWindowSpecs()));
        window->init();
        window->setEventCallback([this](Event& event) { HandleEvent(event); });

        if (specification.maximized)
            window->maximise();

        initializeGraphics();
    }

    Engine::~Engine() {
        window->setEventCallback([](Event& event) {});
        window->terminate();
        instance = nullptr;
    }

    void Engine::initializeRenderResources() {
        // Basic OpenGL setup
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (specs.renderer) {
            // Initialize shader
            try {
                renderResources.shader = std::make_unique<Render::Shader>(
                    default_vert_shader, default_frag_shader);
            } catch (const std::exception& e) {
                std::cerr << "Failed to create shader: " << e.what() << std::endl;
                return;
            }

            // Initialize quad buffer
            renderResources.quad = Render::Buffer::createQuad();

            // Load texture
            try {
                renderResources.texture = std::make_unique<Render::Texture>(
                    "res/image.png", Render::TextureParameters{});
            } catch (const std::exception& e) {
                std::cerr << "Failed to load texture: " << e.what() << std::endl;
            }

            updateProjectionMatrix();
        }
    }

    void Engine::processInput() {
        auto* windowHandle = static_cast<GLFWwindow*>(window->getNativeWindow());

        if (glfwGetKey(windowHandle, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            isTerminated = true;

        // Movement controls
        if (glfwGetKey(windowHandle, GLFW_KEY_W) == GLFW_PRESS)
            position.y += 2.0f * deltaTime;
        if (glfwGetKey(windowHandle, GLFW_KEY_S) == GLFW_PRESS)
            position.y -= 2.0f * deltaTime;
        if (glfwGetKey(windowHandle, GLFW_KEY_A) == GLFW_PRESS)
            position.x -= 2.0f * deltaTime;
        if (glfwGetKey(windowHandle, GLFW_KEY_D) == GLFW_PRESS)
            position.x += 2.0f * deltaTime;

        // Scale controls
        if (glfwGetKey(windowHandle, GLFW_KEY_Q) == GLFW_PRESS) {
            scale.x -= 0.5f * deltaTime;
            scale.y -= 0.5f * deltaTime;
        }
        if (glfwGetKey(windowHandle, GLFW_KEY_E) == GLFW_PRESS) {
            scale.x += 0.5f * deltaTime;
            scale.y += 0.5f * deltaTime;
        }

        // Rotation controls
        if (glfwGetKey(windowHandle, GLFW_KEY_R) == GLFW_PRESS)
            rotationSpeed += 50.0f * deltaTime;
        if (glfwGetKey(windowHandle, GLFW_KEY_F) == GLFW_PRESS)
            rotationSpeed -= 50.0f * deltaTime;

        static bool spaceWasPressed = false;
        if (glfwGetKey(windowHandle, GLFW_KEY_SPACE) == GLFW_PRESS) {
            if (!spaceWasPressed) {
                rotationSpeed = (rotationSpeed != 0.0f) ? 0.0f : 50.0f;
                spaceWasPressed = true;
            }
        } else {
            spaceWasPressed = false;
        }
    }

    void Engine::updateTransforms() {
        rotationAngle += rotationSpeed * deltaTime;
        if (rotationAngle > 360.0f)
            rotationAngle -= 360.0f;
    }

    void Engine::initializeGraphics() {
        // Basic OpenGL setup
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (specs.renderer) {
            initializeRenderResources();
        }
    }

    void Engine::updateProjectionMatrix() {
        glm::vec2 windowSize = window->getSize();
        float width = windowSize.x;
        float height = windowSize.y;
        float aspect = width / static_cast<float>(height);

        // Perspective projection for 3D
        renderResources.projection = glm::perspective(
            glm::radians(45.0f), aspect, 0.1f, 1000.0f);
    }

    void Engine::render() {
        if (!specs.renderer || !renderResources.shader || !renderResources.quad) {
            return;
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        renderResources.shader->use();

        // Update view matrix (will be useful for 3D camera later)
        renderResources.view = glm::mat4(1.0f);

        // Create model matrix
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, position);
        model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
        model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
        model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
        model = glm::scale(model, scale);

        // Set uniforms
        renderResources.shader->setMat4("projection", renderResources.projection);
        renderResources.shader->setMat4("view", renderResources.view);
        renderResources.shader->setMat4("model", model);

        if (renderResources.texture) {
            renderResources.texture->bind(0);
            renderResources.shader->setInt("texture1", 0);
            renderResources.shader->setInt("useTexture", 1);
        } else {
            renderResources.shader->setInt("useTexture", 0);
        }

        renderResources.quad->draw();

        if (renderResources.texture) {
            renderResources.texture->unbind();
        }
    }

    void Engine::Initialize() {
        OnInitialization();
    }

    void Engine::Run() {
        while (!isTerminated) {
            float currentTime = static_cast<float>(glfwGetTime());
            deltaTime = currentTime - lastFrame;
            lastFrame = currentTime;
            deltaTime *= timeScale;

            processInput();
            updateTransforms();

            if (!isMinimized) {
                OnUpdate(deltaTime);
                render();
                OnRender();
                window->swapBuffers();
            }

            window->processEvents();
        }
    }

    void Engine::Terminate() {
        if (isTerminated) return;
        isTerminated = true;
        OnTermination();
    }

    void Engine::HandleEvent(Event& event) {
        switch (event.GetEventType()) {
            case EventType::WindowResize: {
                auto& resizeEvent = static_cast<WindowResizeEvent&>(event);
                glm::vec2 size = resizeEvent.GetSize();
                if (size.x == 0 || size.y == 0) {
                    isMinimized = true;
                } else {
                    isMinimized = false;
                    glViewport(0, 0, size.x, size.y);
                    projection = glm::perspective(glm::radians(45.0f),
                                               size.x / size.y,
                                               0.1f, 100.0f);
                }
                break;
            }
            case EventType::WindowClose: {
                Terminate();
                break;
            }
        }

        OnEvent(event);
    }
}
