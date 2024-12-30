//
// Created by Administrator on 5/19/2024.
//

#ifndef RENDERER_H
#define RENDERER_H
#include <constants.hpp>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <api/renderapi.h>
#include <api/opengl/oglbackend.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace Backend {

    struct WindowMode {
        int width;
        int height;
        bool fullscreen;
    };

    enum class BackendAPI {
        OpenGL,
        Vulkan
    };

    class Logger;
    class Renderer {
    public:
        static void init(BackendAPI api);
        static void beginFrame();
        static void endFrame();
        static void updateSubSystems();
        static void cleanUp();

        static void setAPI(BackendAPI api);
        static const BackendAPI& getAPI();

        static GLFWwindow* getWindowPointer();
        static void setWindowPointer(GLFWwindow* window);

        static void createWindow(const WindowMode& windowMode);
        static void setWindowedMode(const WindowMode& windowMode);
        static void toggleFullscreen();
        static void forceCloseWindow();

        static bool windowHasFocus();
        static bool windowHasNotBeenForceClosed();
        static int getWindowedWidth();
        static int getWindowedHeight();
        static int getFullScreenWidth();
        static int getFullScreenHeight();
        static int getCurrentWindowWidth();
        static int getCurrentWindowHeight();
        static bool windowIsOpen();
        static bool windowIsMinimized();
        static const WindowMode& getWindowMode();

        static void setPresentTargetSize(int width, int height);
        static int getPresentTargetWidth();
        static int getPresentTargetHeight();

        static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
        static void windowFocusCallback(GLFWwindow* window, int focused);

    private:
        static BackendAPI api;
        static GLFWwindow* window;
        static WindowMode windowMode;
        static GLFWmonitor* monitor;
        static const GLFWvidmode* mode;
        static bool forceCloseWindowFlag;
        static bool windowHasFocusFlag;
        static int windowedWidth;
        static int windowedHeight;
        static int fullscreenWidth;
        static int fullscreenHeight;
        static int currentWindowWidth;
        static int currentWindowHeight;
        static int presentTargetWidth;
        static int presentTargetHeight;

        static std::unique_ptr<OGLBackend> oglBackend_;
        static RenderAPI* renderAPI;

    };
}


#endif //RENDERER_H
