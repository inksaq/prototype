//
// Created by Administrator on 5/19/2024.
//

#include "renderer.h"
#include "render/api/opengl/oglbackend.h"
//#include "render/api/vulkan/vkbackend.h"

#include <iostream>
namespace Backend
{

    BackendAPI Renderer::api;
    GLFWwindow* Renderer::window = nullptr;
    WindowMode Renderer::windowMode;
    GLFWmonitor* Renderer::monitor = nullptr;
    const GLFWvidmode* Renderer::mode = nullptr;
    bool Renderer::forceCloseWindowFlag = false;
    bool Renderer::windowHasFocusFlag = true;
    int Renderer::windowedWidth = 800;
    int Renderer::windowedHeight = 600;
    int Renderer::fullscreenWidth = 1920;
    int Renderer::fullscreenHeight = 1080;
    int Renderer::currentWindowWidth = 800;
    int Renderer::currentWindowHeight = 600;
    int Renderer::presentTargetWidth = 800;
    int Renderer::presentTargetHeight = 600;
    std::unique_ptr<OGLBackend> Renderer::oglBackend_;
    RenderAPI* Renderer::renderAPI = nullptr;

    void Renderer::init(BackendAPI selectedApi) {
        api = selectedApi;

        // Initialize GLFW
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }

        // Set GLFW window hints based on the selected API
        if (api == BackendAPI::OpenGL) {
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        }

        createWindow(windowMode);

        // Initialize the backend
        if (api == BackendAPI::OpenGL) {
            oglBackend_ = std::make_unique<OGLBackend>();
            renderAPI = oglBackend_.get();
            oglBackend_->init();
            oglBackend_->loadShaders();
            oglBackend_->loadBuffers();
            oglBackend_->loadTextures();
            oglBackend_->initFramebuffer();
        }

        glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
        glfwSetWindowFocusCallback(window, windowFocusCallback);
    }

    void Renderer::beginFrame() {
        if (api == BackendAPI::OpenGL) {
            // OpenGL specific frame initialization
        }
    }

    void Renderer::endFrame() {
        if (api == BackendAPI::OpenGL) {
            glfwSwapBuffers(window);
        }
        glfwPollEvents();
    }

    void Renderer::updateSubSystems() {
        // Update any necessary subsystems here
    }

    void Renderer::cleanUp() {
        if (api == BackendAPI::OpenGL) {
            oglBackend_->shutdown();
        }
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    void Renderer::setAPI(BackendAPI newApi) {
        api = newApi;
    }

    const BackendAPI& Renderer::getAPI() {
        return api;
    }

    GLFWwindow* Renderer::getWindowPointer() {
        return window;
    }

    void Renderer::setWindowPointer(GLFWwindow* newWindow) {
        window = newWindow;
    }

    void Renderer::createWindow(const WindowMode& mode) {
        windowMode = mode;
        monitor = glfwGetPrimaryMonitor();
        Renderer::mode = glfwGetVideoMode(monitor);

        if (windowMode.fullscreen) {
            window = glfwCreateWindow(mode.width, mode.height, "My Engine", monitor, nullptr);
            fullscreenWidth = mode.width;
            fullscreenHeight = mode.height;
        } else {
            window = glfwCreateWindow(windowedWidth, windowedHeight, "My Engine", nullptr, nullptr);
        }

        if (!window) {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return;
        }

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable v-sync
    }

    void Renderer::setWindowedMode(const WindowMode& mode) {
        windowMode = mode;
        glfwSetWindowSize(window, mode.width, mode.height);
    }

    void Renderer::toggleFullscreen() {
        if (windowMode.fullscreen) {
            setWindowedMode({windowedWidth, windowedHeight, false});
        } else {
            setWindowedMode({fullscreenWidth, fullscreenHeight, true});
        }
        windowMode.fullscreen = !windowMode.fullscreen;
    }

    void Renderer::forceCloseWindow() {
        forceCloseWindowFlag = true;
    }

    bool Renderer::windowHasFocus() {
        return windowHasFocusFlag;
    }

    bool Renderer::windowHasNotBeenForceClosed() {
        return !forceCloseWindowFlag;
    }

    int Renderer::getWindowedWidth() {
        return windowedWidth;
    }

    int Renderer::getWindowedHeight() {
        return windowedHeight;
    }

    int Renderer::getFullScreenWidth() {
        return fullscreenWidth;
    }

    int Renderer::getFullScreenHeight() {
        return fullscreenHeight;
    }

    int Renderer::getCurrentWindowWidth() {
        return currentWindowWidth;
    }

    int Renderer::getCurrentWindowHeight() {
        return currentWindowHeight;
    }

    bool Renderer::windowIsOpen() {
        return !glfwWindowShouldClose(window);
    }

    bool Renderer::windowIsMinimized() {
        return glfwGetWindowAttrib(window, GLFW_ICONIFIED);
    }

    const WindowMode& Renderer::getWindowMode() {
        return windowMode;
    }

    void Renderer::setPresentTargetSize(int width, int height) {
        presentTargetWidth = width;
        presentTargetHeight = height;
    }

    int Renderer::getPresentTargetWidth() {
        return presentTargetWidth;
    }

    int Renderer::getPresentTargetHeight() {
        return presentTargetHeight;
    }

    void Renderer::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
        currentWindowWidth = width;
        currentWindowHeight = height;
        if (api == BackendAPI::OpenGL) {
            glViewport(0, 0, width, height);
        }
    }

    void Renderer::windowFocusCallback(GLFWwindow* window, int focused) {
        windowHasFocusFlag = focused;
    }
}