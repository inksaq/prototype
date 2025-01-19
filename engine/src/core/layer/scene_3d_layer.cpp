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
#include <shaders/grid_shaders.hh>

namespace Core::Engine {
    Scene3DLayer::Scene3DLayer() : Layer("Scene3DLayer") {
    std::cout << "Scene3DLayer constructor - Starting..." << std::endl;
    try {
        // Any constructor initialization here
        std::cout << "Scene3DLayer constructor - Completed" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Scene3DLayer constructor - Failed with error: " << e.what() << std::endl;
        throw;
    }
}

    void Scene3DLayer::OnAttach() {
        std::cout << "Scene3DLayer::OnAttach - Starting..." << std::endl;
        try {
            initializeRenderResources();
            std::cout << "Scene3DLayer::OnAttach - Completed successfully" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Scene3DLayer::OnAttach - Failed with error: " << e.what() << std::endl;
            throw; // Re-throw to maintain error propagation
        }
    }

    void Scene3DLayer::initializeRenderResources() {
    std::cout << "Scene3DLayer::initializeRenderResources - Starting..." << std::endl;

    // Basic OpenGL setup
    std::cout << "Setting up OpenGL states..." << std::endl;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    try {
        // Create VAO first
        std::cout << "Creating VAO..." << std::endl;
        renderResources.gridVAO = std::make_unique<Render::VertexArray>();
        if (!renderResources.gridVAO) {
            throw std::runtime_error("Failed to create grid VAO");
        }

        // Initialize grid shader
        std::cout << "Creating grid shader..." << std::endl;
        renderResources.gridShader = std::make_unique<Render::Shader>(
            gridShadersPackage.at("grid")[0],
            gridShadersPackage.at("grid")[1]);
        std::cout << "Grid shader created successfully" << std::endl;

        // Create grid geometry
        std::cout << "Generating grid vertices..." << std::endl;
        std::vector<float> gridVertices;
        float size = 50.0f;      // Larger grid
        float step = 5.0f;       // Wider spacing

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
        renderResources.gridVertexCount = gridVertices.size() / 3;
        std::cout << "Generated " << renderResources.gridVertexCount << " vertices for grid" << std::endl;

        // Create vertex buffer
        std::cout << "Creating vertex buffer..." << std::endl;
        renderResources.gridBuffer = Core::Render::Buffer::createVertexBuffer(
            gridVertices.data(),
            static_cast<uint32_t>(gridVertices.size() * sizeof(float))
        );

        if (!renderResources.gridBuffer) {
            throw std::runtime_error("Failed to create grid vertex buffer");
        }

        // Set buffer layout
        std::cout << "Setting buffer layout..." << std::endl;
        Core::Render::BufferLayout layout = {
            { Core::Render::ShaderDataType::Float3, "aPos" }
        };
        renderResources.gridBuffer->setLayout(layout);

        // Add buffer to VAO
        std::cout << "Binding VAO and adding vertex buffer..." << std::endl;
        renderResources.gridVAO->bind();
        renderResources.gridVAO->addVertexBuffer(renderResources.gridBuffer);
        renderResources.gridVAO->unbind();

        // Initialize camera position and matrices
        std::cout << "Initializing camera and matrices..." << std::endl;

        // Set up camera and grid properties
        renderResources.position = glm::vec3(50.0f, 50.0f, 50.0f);  // Position camera at an angle
        renderResources.scale = glm::vec3(1.0f);
        renderResources.gridColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);  // Solid white
        renderResources.fadeDistance = 500.0f;  // Large fade distance


        updateProjectionMatrix();
        updateTransforms(0.0f);
        initialized = true;
        std::cout << "Scene3DLayer::initializeRenderResources - Completed successfully" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Scene3DLayer::initializeRenderResources - Failed with error: " << e.what() << std::endl;
        throw; // Re-throw to maintain error propagation
    }
}

    void Scene3DLayer::updateProjectionMatrix() {
        auto& window = Engine::Get().GetWindow();
        glm::vec2 size = window.getSize();
        float aspect = size.x / size.y;

        // Use wider FOV and adjusted near/far planes
        renderResources.projection = glm::perspective(
            glm::radians(45.0f),  // FOV
            aspect,
            0.1f,                 // Near plane
            1000.0f              // Far plane
        );

        // Debug output
        std::cout << "Projection Matrix updated:" << std::endl;
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                std::cout << renderResources.projection[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }


    void Scene3DLayer::processInput(float deltaTime) {
        auto& window = Engine::Get().GetWindow();
        auto* windowHandle = static_cast<GLFWwindow*>(window.getNativeWindow());

        float velocity = moveSpeed * deltaTime;

        // Camera movement using keys
        if (glfwGetKey(windowHandle, GLFW_KEY_W) == GLFW_PRESS)
            renderResources.position += renderResources.front * velocity;
        if (glfwGetKey(windowHandle, GLFW_KEY_S) == GLFW_PRESS)
            renderResources.position -= renderResources.front * velocity;
        if (glfwGetKey(windowHandle, GLFW_KEY_A) == GLFW_PRESS)
            renderResources.position -= renderResources.right * velocity;
        if (glfwGetKey(windowHandle, GLFW_KEY_D) == GLFW_PRESS)
            renderResources.position += renderResources.right * velocity;
        if (glfwGetKey(windowHandle, GLFW_KEY_Q) == GLFW_PRESS)
            renderResources.position -= renderResources.up * velocity;
        if (glfwGetKey(windowHandle, GLFW_KEY_E) == GLFW_PRESS)
            renderResources.position += renderResources.up * velocity;

        // Camera rotation using mouse
        double mouseX, mouseY;
        glfwGetCursorPos(windowHandle, &mouseX, &mouseY);

        if (firstMouse) {
            lastX = mouseX;
            lastY = mouseY;
            firstMouse = false;
        }

        float xoffset = mouseX - lastX;
        float yoffset = lastY - mouseY; // Reversed since y-coordinates range from bottom to top
        lastX = mouseX;
        lastY = mouseY;

        // Only process mouse movement if right button is held
        if (glfwGetMouseButton(windowHandle, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
            xoffset *= mouseSensitivity;
            yoffset *= mouseSensitivity;

            yaw += xoffset;
            pitch += yoffset;

            // Constrain pitch
            if (pitch > 89.0f)
                pitch = 89.0f;
            if (pitch < -89.0f)
                pitch = -89.0f;
        }
    }

    void Scene3DLayer::updateTransforms(float deltaTime) {
        // Calculate camera vectors
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        renderResources.front = glm::normalize(direction);
        renderResources.right = glm::normalize(glm::cross(renderResources.front, renderResources.up));

        // Update view matrix
        renderResources.view = glm::lookAt(
            renderResources.position,
            renderResources.position + renderResources.front,
            renderResources.up
        );

        // Debug output for view matrix
        std::cout << "View Matrix updated:" << std::endl;
        for(int i = 0; i < 4; i++) {
            for(int j = 0; j < 4; j++) {
                std::cout << renderResources.view[i][j] << " ";
            }
            std::cout << std::endl;
        }
    }

    void Scene3DLayer::setGridProperties(const GridSettings &settings) {
        renderResources.gridColor = settings.color;
        renderResources.fadeDistance = settings.fadeDistance;
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

        std::cout << "\nScene3DLayer::OnRender - Starting render cycle" << std::endl;
        // Save current OpenGL state
        GLboolean depthTestWasEnabled;
        glGetBooleanv(GL_DEPTH_TEST, &depthTestWasEnabled);

        // Ensure proper depth testing
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);  // Changed to LEQUAL for better depth handling
        glDepthMask(GL_TRUE);    // Ensure depth buffer is writable

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        try {
            renderResources.gridShader->use();

            // Combine view and projection matrices
            updateProjectionMatrix();  // Update every frame for now
            glm::mat4 viewProjection = renderResources.projection * renderResources.view;
            renderResources.gridShader->setMat4("uViewProjection", viewProjection);
            renderResources.gridShader->setVec4("uGridColor", renderResources.gridColor);
            renderResources.gridShader->setFloat("uFadeDistance", renderResources.fadeDistance);

            // Draw grid
            renderResources.gridVAO->bind();
            glLineWidth(2.0f);  // Make lines thicker
            glDrawArrays(GL_LINES, 0, static_cast<GLsizei>(renderResources.gridVertexCount));
            renderResources.gridVAO->unbind();

            // Check for errors
            GLenum err;
            while ((err = glGetError()) != GL_NO_ERROR) {
                std::cerr << "OpenGL error after grid render: 0x" << std::hex << err << std::dec << std::endl;
            }

        } catch (const std::exception& e) {
            std::cerr << "Error during grid rendering: " << e.what() << std::endl;
        }

        std::cout << "Scene3DLayer::OnRender - Completed render cycle" << std::endl;
        // Restore previous depth test state
        if (!depthTestWasEnabled) {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void Scene3DLayer::OnImGuiRender() {
        ImGui::Begin("Scene3D Settings");

        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::DragFloat3("Position", &renderResources.position[0], 0.1f);
        }

        ImGui::End();
    }

    void Scene3DLayer::OnEvent(Event& event) {
        switch (event.GetEventType()) {
            case EventType::WindowResize: {
                auto& resizeEvent = static_cast<WindowResizeEvent&>(event);
                glm::vec2 size = resizeEvent.GetSize();
                if (size.x > 0 && size.y > 0) {
                    updateProjectionMatrix();
                }
                break;
            }
        }
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
