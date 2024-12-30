//
// Created by Administrator on 5/19/2024.
//

#include "oglbackend.h"

#include <iostream>
#include <memory>
#include <renderer.h>
#include <stdexcept>
#include <textblitter.h>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "oglassetmanager.h"
#include "buffer/buffer.h"
#include "buffer/vertexarray.h"

namespace Backend {


    void OGLBackend::init() {
        // Initialize OpenGL, glad, etc.
        // This might include context setup, function pointer loading, etc.
        // ...

        // Load resources
        loadShaders();
        loadBuffers();
        loadTextures();
        initFramebuffer();
    }

    void OGLBackend::shutdown() {
        // Cleanup resources
        // This might include deleting OpenGL objects, freeing memory, etc.
        // ...
    }

     void OGLBackend::loadShaders() {
        // Example shader loading
        shaders_.emplace(1, OGLAsse
            tManager::getInstance().loadShader("path/to/vertex.glsl", "path/to/fragment.glsl"));
        // Add more shaders as needed
    }

    void OGLBackend::loadBuffers() {
        // Example buffer loading
        std::vector<float> vertexData = {
            // Vertex data goes here
        };
        buffers_.emplace(1, backend::renderapi::opengl::OGLAssetManager::getInstance().loadBuffer(vertexData, GL_STATIC_DRAW));

        std::vector<uint32_t> indexData = {
            // Index data goes here
        };
        buffers_.emplace(2, backend::renderapi::opengl::OGLAssetManager::getInstance().loadBuffer(indexData, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER));
    }

    void OGLBackend::loadTextures() {
        // Example texture loading
        textures_.emplace(1, backend::renderapi::opengl::OGLAssetManager::getInstance().loadTexture("path/to/texture.png"));
        // Add more textures as needed
    }

    void OGLBackend::initFramebuffer() {
        int width = 800;  // Replace with your desired width
        int height = 600; // Replace with your desired height
        framebuffers_.emplace(1, backend::renderapi::opengl::OGLAssetManager::getInstance().createFramebuffer(width, height));
    }

    void OGLBackend::UploadVertexData(std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
        // Create and bind the VAO
        auto vertexArray = backend::renderapi::opengl::OGLAssetManager::getInstance().createVertexArray();
        vertexArray->bind();

        // Create and bind the vertex buffer
        std::vector<float> vertexData(vertices.size() * 11); // 3 position, 3 normal, 2 uv, 3 tangent
        for (size_t i = 0; i < vertices.size(); ++i) {
            vertexData[i * 11 + 0] = vertices[i].position.x;
            vertexData[i * 11 + 1] = vertices[i].position.y;
            vertexData[i * 11 + 2] = vertices[i].position.z;
            vertexData[i * 11 + 3] = vertices[i].normal.x;
            vertexData[i * 11 + 4] = vertices[i].normal.y;
            vertexData[i * 11 + 5] = vertices[i].normal.z;
            vertexData[i * 11 + 6] = vertices[i].uv.x;
            vertexData[i * 11 + 7] = vertices[i].uv.y;
            vertexData[i * 11 + 8] = vertices[i].tangent.x;
            vertexData[i * 11 + 9] = vertices[i].tangent.y;
            vertexData[i * 11 + 10] = vertices[i].tangent.z;
        }
        auto vertexBuffer = backend::renderapi::opengl::OGLAssetManager::getInstance().loadBuffer(vertexData, GL_STATIC_DRAW);

        // Create and bind the index buffer
        auto indexBuffer = backend::renderapi::opengl::OGLAssetManager::getInstance().loadBuffer(indices, GL_STATIC_DRAW, GL_ELEMENT_ARRAY_BUFFER);

        // Set vertex attribute pointers
        vertexArray->setAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        vertexArray->setAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
        vertexArray->setAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
        vertexArray->setAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tangent));

        // Store the VAO
        vertexArrays_.emplace(1, vertexArray);

        // Unbind VAO to prevent accidental modification
        vertexArray->unbind();
    }

    void OGLBackend::renderLoadingScreen() {
        auto& loadingScreenFB = framebuffers_.at(1);
        loadingScreenFB->bind();
        glViewport(0, 0, loadingScreenFB->getWidth(), loadingScreenFB->getHeight());
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_CULL_FACE);

        glClearColor(0, 0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);

        auto& shader = shaders_.at(1);
        shader->use();
        shader->setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->setVec3("overrideColor", glm::vec3(1.0f, 1.0f, 1.0f));
        shader->setMat4("model", glm::mat4(1.0f));

        std::string text = "";
        int maxLinesDisplayed = 40;
        int endIndex = OpenGLAssetManager::_loadLog.size();
        int beginIndex = std::max(0, endIndex - maxLinesDisplayed);
        for (int i = beginIndex; i < endIndex; i++) {
            text += OpenGLAssetManager::_loadLog[i] + "\n";
        }
        TextBlitter::_debugTextToBilt = text;
        TextBlitter::CreateRenderItems(loadingScreenFB->getWidth(), loadingScreenFB->getHeight());
        renderUI();

        glViewport(0, 0, Renderer::getCurrentWindowWidth(), Renderer::getCurrentWindowHeight());
        loadingScreenFB->bindRead();
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glReadBuffer(GL_COLOR_ATTACHMENT0);
        glBlitFramebuffer(0, 0, loadingScreenFB->getWidth(), loadingScreenFB->getHeight(), 0, 0, Renderer::getCurrentWindowWidth(), Renderer::getCurrentWindowHeight(), GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OGLBackend::renderUI() {
        // Example UI rendering code
        // This function should render the actual UI elements to the screen
    }
};