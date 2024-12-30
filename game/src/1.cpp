#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdio>
#include <iostream>
#include <engine/includes/render/texture.h>
#include <engine/includes/render/buffer.h>
#include "engine/includes/core/engine.h"
#include "engine/includes/engine.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(1600, 900, "Hello Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    // engine.started = true;

    // Make the window's context current
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable v-sync

    // Initialize GLEW
    glewExperimental = GL_TRUE; // Needed for core profile
    if (GLEW_OK != glewInit()) {
        std::cerr << "Failed to initialize GLEW\n";
        return -1;
    }

    // Configure viewport
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // ShaderManager setup

    // ShaderManager setup
    Core::Render::Shader shader(default_vert_shader, default_frag_shader);

    // Texture loading with error handling and absolute path
    Core::Render::Texture* texture = nullptr;
    try {
        // Use a known path relative to your project or executable
        const char* texturePath = "res/image.png";  // Adjust this path
        texture = new Core::Render::Texture(texturePath);
        std::cout << "Texture loaded successfully: " << texturePath << std::endl;
        std::cout << "Texture dimensions: " << texture->getWidth() << "x" << texture->getHeight()
                  << " channels: " << texture->getChannels() << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "Failed to load texture: " << e.what() << std::endl;
        return -1;
    }

    auto quad = Core::Render::Buffer::createQuad();
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)width / (float)height, 0.1f, 100.0f);

    // Create model matrix
    glm::mat4 model = glm::mat4(1.0f); // Identity matrix
    // If you want to transform the quad, modify the model matrix here
    // For example, to scale it down:
    model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));



    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        if (texture) {
            shader.use();

            // Set transformation matrices
            shader.setMat4("projection", projection);
            shader.setMat4("view", camera.GetViewMatrix());
            shader.setMat4("model", model);

            // Set texture uniforms
            texture->bind(0);
            shader.setInt("texture1", 0);
            shader.setInt("useTexture", 1);  // Tell shader to use texture

            // Draw
            quad->draw();

            // Unbind
            texture->unbind();
        } else {
            // If texture loading failed, draw with color
            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", camera.GetViewMatrix());
            shader.setMat4("model", model);
            shader.setInt("useTexture", 0);  // Tell shader to use color

            quad->draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // rectVao.Delete();

    delete texture;
    // Terminate GLFW
    glfwTerminate();

    return 0;
}

