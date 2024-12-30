//
// Created by Administrator on 12/29/2024.
//

#ifndef WINDOW_H
#define WINDOW_H

#include <functional>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#include <GLFW/glfw3native.h>
#include <filesystem>
#include <string>
#include <glm/glm.hpp>

#include "event/event.h"

namespace Core::Engine {
    struct WindowSpecs {
        std::string title = "EngineV1";
        std::filesystem::path iconPath;
        int32_t width = 1600, height = 900;
        int32_t x = -1, y = -1;  // -1 means centered
        bool decorated = true;
        bool fullscreen = false;
        bool maximised = false;
        bool darkMode = true;
        bool vSync = true;
        bool resizable = true;
        int32_t minWidth = 800;   // Added minimum size
        int32_t minHeight = 600;
        int32_t samples = 4;      // MSAA samples
    };

    class Window {
        using EventCallbackFunc = std::function<void(Event&)>;
    private:
        GLFWwindow* window;
        WindowSpecs specs;
        EventCallbackFunc eventCallback;
        bool vsyncEnabled;

        void setupWindowHints();
        void setupCallbacks();
        bool createWindow();
        void setupIcon();

    public:
        Window(const WindowSpecs& specs) : specs(specs) {};
        ~Window() {}

        // Prevent copying
        Window(const Window&) = delete;
        Window& operator=(const Window&) = delete;

        bool init();
        void terminate();

        void processEvents();
        void swapBuffers();

        void maximise();
        void center();
        void setFullscreen(bool fullscreen);

        // Window manipulation
        void setSize(const glm::vec2& size);
        glm::vec2 getSize();
        void setPosition(const glm::vec2& position);
        glm::vec2 getPosition();
        void setTitle(const std::string& title);
        const std::string& getTitle();

        // State queries
        bool isMinimized() const;
        bool isMaximized() const;
        bool isFullscreen() const;
        bool shouldClose() const;

        // Event handling
        void setEventCallback(const EventCallbackFunc& callback);
        EventCallbackFunc getEventCallback();

        void setVsync(bool enabled);
        bool isVsync() ;

        // Native window handles
        inline GLFWwindow* getNativeWindow() const { return window; }
        inline HWND getHWND() const { return glfwGetWin32Window(window); }
        inline float getTime() const { return static_cast<float>(glfwGetTime()); }

        static Window* create(const WindowSpecs& specs = WindowSpecs());


    };
}

#endif // WINDOW_H
