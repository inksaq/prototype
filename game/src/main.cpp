#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "engine/includes/core/engine.h"
#include "engine/includes/engine.hpp"
#include "glm/fwd.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


Core::Engine::Engine engine;

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
    GLFWwindow* window = glfwCreateWindow(640, 480, "Hello Triangle", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    engine.started = true;

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

    // ShaderManager setup
    ShaderManager shaderManager;
    auto defaultShaderProgram = shaderManager.loadShaderFromMemory(default_vert_shader, default_frag_shader, "default");
    // Define the vertex data for a triangle
    RectVAO rectVao;
    Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom),
            (float)width / (float)height, 0.1f, 100.0f);


    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Use the shader program
        glUseProgram(defaultShaderProgram);
        unsigned int viewLoc = glGetUniformLocation(defaultShaderProgram, "view");
        unsigned int projLoc = glGetUniformLocation(defaultShaderProgram, "projection");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

        // Draw the triangle
        glBindVertexArray(rectVao.VBO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
    rectVao.Delete();

    // Terminate GLFW
    glfwTerminate();

    return 0;
}

