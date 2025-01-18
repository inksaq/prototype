//
// Created by admin on 12/01/2025.
//


#include <imgui.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "core/layer/scene_3d_layer.h"
#include "core/input.h"
#include "core/engine.h"

#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui.h>
#include <GLFW/glfw3.h>

namespace Core::Engine {
    Scene3DLayer::Scene3DLayer() : Layer("Scene3DLayer") {
        std::cout << "Creating Scene3DLayer..." << std::endl;
    }

    void Scene3DLayer::OnAttach() {
        std::cout << "Initializing Scene3DLayer..." << std::endl;
        initializeRenderResources();
    }

    void Scene3DLayer::initializeRenderResources() {
        std::cout << "Initializing render resources..." << std::endl;

        // Basic OpenGL setup
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Initialize grid shader
        try {
            const char* gridVertexShader = R"(
                #version 410 core
                layout (location = 0) in vec3 aPos;
                uniform mat4 projection;
                uniform mat4 view;
                uniform mat4 model;

                void main() {
                    gl_Position = projection * view * model * vec4(aPos, 1.0);
                }
            )";

            const char* gridFragmentShader = R"(
                #version 410 core
                out vec4 FragColor;

                void main() {
                    FragColor = vec4(0.5, 0.5, 0.5, 0.5);
                }
            )";

            renderResources.gridShader = std::make_unique<Render::Shader>(
                gridVertexShader, gridFragmentShader);
            std::cout << "Grid shader created successfully" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Failed to create grid shader: " << e.what() << std::endl;
            return;
        }

        // Create grid geometry
        std::vector<float> gridVertices;
        float size = 10.0f;
        float step = 1.0f;

        // Generate grid lines
        for (float i = -size; i <= size; i += step) {
            // Vertical lines
            gridVertices.push_back(i);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(-size);

            gridVertices.push_back(i);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(size);

            // Horizontal lines
            gridVertices.push_back(-size);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(i);

            gridVertices.push_back(size);
            gridVertices.push_back(0.0f);
            gridVertices.push_back(i);
        }
        renderResources.gridVertexCount = gridVertices.size() / 3; // 3 components per vertex
        // Create and configure vertex array

        try {

            // Create vertex buffer
            renderResources.gridBuffer = Core::Render::Buffer::createVertexBuffer(
                gridVertices.data(),
                static_cast<uint32_t>(gridVertices.size() * sizeof(float))
            );

            if (!renderResources.gridBuffer) {
                std::cerr << "Failed to create vertex buffer" << std::endl;
                return;
            }

            std::cout << "Created vertex buffer with " << renderResources.gridVertexCount << " vertices" << std::endl;

            // Set buffer layout
            Core::Render::BufferLayout layout = {
                { Core::Render::ShaderDataType::Float3, "aPos" }
            };
            renderResources.gridBuffer->setLayout(layout);

            // Add buffer to VAO
            renderResources.gridVAO->bind();
            renderResources.gridVAO->addVertexBuffer(renderResources.gridBuffer);
            renderResources.gridVAO->unbind();
        // Initialize camera position and matrices
        renderResources.position = glm::vec3(0.0f, 5.0f, 10.0f);
        renderResources.scale = glm::vec3(1.0f);
        updateProjectionMatrix();

        initialized = true;
        std::cout << "Render resources initialization complete" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize render resources: " << e.what() << std::endl;
        return;
    }
    }

    void Scene3DLayer::updateProjectionMatrix() {
        auto& window = Engine::Get().GetWindow();
        glm::vec2 size = window.getSize();
        float aspect = size.x / size.y;

        renderResources.projection = glm::perspective(glm::radians(45.0f), aspect, 0.1f, 100.0f);
        renderResources.view = glm::lookAt(
            renderResources.position,
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f)
        );
    }

    void Scene3DLayer::processInput(float deltaTime) {
        auto& window = Engine::Get().GetWindow();
        auto* windowHandle = static_cast<GLFWwindow*>(window.getNativeWindow());

        float moveSpeed = 5.0f * deltaTime;

        // Camera movement
        if (glfwGetKey(windowHandle, GLFW_KEY_W) == GLFW_PRESS)
            renderResources.position.z -= moveSpeed;
        if (glfwGetKey(windowHandle, GLFW_KEY_S) == GLFW_PRESS)
            renderResources.position.z += moveSpeed;
        if (glfwGetKey(windowHandle, GLFW_KEY_A) == GLFW_PRESS)
            renderResources.position.x -= moveSpeed;
        if (glfwGetKey(windowHandle, GLFW_KEY_D) == GLFW_PRESS)
            renderResources.position.x += moveSpeed;
        if (glfwGetKey(windowHandle, GLFW_KEY_Q) == GLFW_PRESS)
            renderResources.position.y -= moveSpeed;
        if (glfwGetKey(windowHandle, GLFW_KEY_E) == GLFW_PRESS)
            renderResources.position.y += moveSpeed;
    }

    void Scene3DLayer::updateTransforms(float deltaTime) {
        updateProjectionMatrix();
    }

    void Scene3DLayer::OnUpdate(float deltaTime) {
        if (!initialized) return;

        processInput(deltaTime);
        updateTransforms(deltaTime);
    }

    void Scene3DLayer::OnRender() {
        if (!initialized || !renderResources.gridShader || !renderResources.gridBuffer) {
            return;
        }

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        try {

        renderResources.gridShader->use();
        renderResources.gridShader->setMat4("projection", renderResources.projection);
        renderResources.gridShader->setMat4("view", renderResources.view);
        renderResources.gridShader->setMat4("model", glm::mat4(1.0f));

        renderResources.gridVAO->bind();
    glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(renderResources.gridVertexCount));
        renderResources.gridVAO->unbind();

        // Check for errors
        GLenum err;
        while ((err = glGetError()) != GL_NO_ERROR) {
            std::cerr << "OpenGL error in OnRender: 0x" << std::hex << err << std::dec << std::endl;
        }

    } catch (const std::exception& e) {
        std::cerr << "Error during rendering: " << e.what() << std::endl;
    }
    }

    void Scene3DLayer::OnImGuiRender() {
        ImGui::Begin("Scene3D Settings");

        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::DragFloat3("Position", &renderResources.position[0], 0.1f);
        }

        ImGui::End();
    }

    void Scene3DLayer::OnDetach() {
        try {
            renderResources.gridBuffer.reset();
            renderResources.gridVAO.reset();
            renderResources.gridShader.reset();
            initialized = false;
        } catch (const std::exception& e) {
            std::cerr << "Error during layer detachment: " << e.what() << std::endl;
        }
    }
}