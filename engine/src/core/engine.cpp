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
        window = std::unique_ptr<Window>(Window::create(createWindowSpecs()));

        if (!window->init()) {
            throw std::runtime_error("Failed to initialise window");
        }

        // Initialize the event callback with a default empty function
        window->setEventCallback([this](Event& event) { HandleEvent(event); });

        window->initializeCallbacks();

        if (specification.maximized)
            window->maximise();

        initializeGraphics();
    }

    Engine::~Engine() {
        renderResources.quad.reset();
        renderResources.texture.reset();
        renderResources.shader.reset();
        window->setEventCallback([](Event& event) {});
        window->terminate();
        instance = nullptr;
    }

    void Engine::initializeRenderResources() {
        std::cout << "Initializing render resources..." << std::endl;

        // Basic OpenGL setup
        glEnable(GL_DEPTH_TEST);


        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (specs.renderer) {
            std::cout << "Renderer enabled, creating resources..." << std::endl;

            // Initialize shader
            try {
                renderResources.shader = std::make_unique<Render::Shader>(
                    default_vert_shader, default_frag_shader);
                std::cout << "Shader created successfully" << std::endl;
            } catch (const std::exception& e) {
                std::cerr << "Failed to create shader: " << e.what() << std::endl;
                return;
            }

            // Initialize quad using the new Buffer system
            renderResources.quad = Render::Buffer::createQuad();
            if (renderResources.quad) {
                std::cout << "Quad vertex array created successfully" << std::endl;
            } else {
                std::cerr << "Failed to create quad vertex array" << std::endl;
                return;
            }

            // Load texture
            try {
                std::filesystem::path texturePath = "res/image.png";
                std::cout << "Attempting to load texture from: "
                          << std::filesystem::absolute(texturePath) << std::endl;

                renderResources.texture = std::make_unique<Render::Texture>(
                    texturePath.string(), Render::TextureParameters{});

                std::cout << "Texture loaded successfully" << std::endl;
                if (renderResources.texture) {
                    std::cout << "Texture dimensions: " << renderResources.texture->getWidth()
                              << "x" << renderResources.texture->getHeight()
                              << " channels: " << renderResources.texture->getChannels() << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Failed to load texture: " << e.what() << std::endl;
            }

            updateProjectionMatrix();
            std::cout << "Render resources initialization complete" << std::endl;
        } else {
            std::cout << "Renderer is disabled in specs" << std::endl;
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

        // For 2D rendering, use orthographic projection instead of perspective
        float orthoSize = 5.0f; // Adjust this value to control how much of the world is visible
        renderResources.projection = glm::ortho(
            -orthoSize * aspect, orthoSize * aspect,
            -orthoSize, orthoSize,
            -1.0f, 1.0f
        );

        // Debug output
        std::cout << "Projection Matrix:" << std::endl;
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                std::cout << renderResources.projection[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

   void Engine::render() {
    if (!specs.renderer || !renderResources.shader || !renderResources.quad) {
        return;
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    renderResources.shader->use();

    // Set up view matrix for 2D rendering
    renderResources.view = glm::mat4(1.0f);

    // Create model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::translate(model, glm::vec3(0.5f * scale.x, 0.5f * scale.y, 0.0f));
    model = glm::rotate(model, glm::radians(rotationAngle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(-0.5f * scale.x, -0.5f * scale.y, 0.0f));
    model = glm::scale(model, scale);

        std::cout << "\nMatrix States:" << std::endl;
        std::cout << "Model Matrix:" << std::endl;
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                std::cout << model[i][j] << " ";
            }
            std::cout << std::endl;
        }

    // Set uniforms
    renderResources.shader->setMat4("projection", renderResources.projection);
    renderResources.shader->setMat4("view", renderResources.view);
    renderResources.shader->setMat4("model", model);

    if (renderResources.texture) {
        // Add debug output for texture state
        GLint boundTexture;
        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
        std::cout << "Before binding - Current bound texture: " << boundTexture << std::endl;

        glActiveTexture(GL_TEXTURE0);
        renderResources.texture->bind(0);

        glGetIntegerv(GL_TEXTURE_BINDING_2D, &boundTexture);
        std::cout << "After binding - Current bound texture: " << boundTexture << std::endl;

        renderResources.shader->setInt("texture1", 0);
        renderResources.shader->setInt("useTexture", 1);
    }

    // Draw the quad with debug info
    renderResources.quad->bind();

        GLint vao, vbo, ebo;
        glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &vao);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &vbo);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &ebo);

        std::cout << "Buffer State:" << std::endl;
        std::cout << "Bound VAO: " << vao << std::endl;
        std::cout << "Bound VBO: " << vbo << std::endl;
        std::cout << "Bound EBO: " << ebo << std::endl;

    // Debug output before draw
    GLint indexCount = renderResources.quad->getIndexBuffer()->getCount();
    std::cout << "Drawing with index count: " << indexCount << std::endl;

    // Verify index buffer binding
    GLint boundElementBuffer;
    glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &boundElementBuffer);
    std::cout << "Bound element buffer: " << boundElementBuffer << std::endl;

    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, nullptr);

    // Check for GL errors after draw
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout << "OpenGL error after draw: 0x" << std::hex << err << std::dec << std::endl;
    }

    renderResources.quad->unbind();

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
