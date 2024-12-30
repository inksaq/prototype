// #include "core/window.h"

#include <iostream>
#include <stb_image.h>
#include <core/window.h>
#include <core/keycodes.h>

#include <core/event/event.h>
#include <core/event/e_key.h>
#include <core/event/e_mouse.h>
#include <core/event/e_window.h>

#define ASSERT(...)

namespace Core::Engine {

    // Window::Window(const WindowSpecs& specs)
    // : window(nullptr), specs(specs), vsyncEnabled(specs.vSync) {
    // }
    //
    // Window::~Window() {
    //     terminate();
    // }

    bool Window::init() {
        static bool glfwInitialized = false;
        if (!glfwInitialized) {
            if (!glfwInit()) {
                std::cerr << "Failed to initialize GLFW\n";
                return false;
            }
            glfwInitialized = true;
        }

        setupWindowHints();
        if (!createWindow()) {
            return false;
        }

        // Set the OpenGL context
        glfwMakeContextCurrent(window);

        // Initialize GLAD
        glewExperimental = GL_TRUE; // Needed for core profile
        if (GLEW_OK != glewInit()) {
            std::cerr << "Failed to initialize GLEW\n";
            return false;
        }

        setupCallbacks();
        setupIcon();

        // Setup vsync
        setVsync(specs.vSync);

        // Set minimum size
        glfwSetWindowSizeLimits(window, specs.minWidth, specs.minHeight, GLFW_DONT_CARE, GLFW_DONT_CARE);

        if (specs.maximised) {
            maximise();
        } else if (specs.x < 0 || specs.y < 0) {
            center();
        }

        return true;
    }

    void Window::setupWindowHints() {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, specs.resizable);
        glfwWindowHint(GLFW_DECORATED, specs.decorated);
        glfwWindowHint(GLFW_SAMPLES, specs.samples);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    }

    bool Window::createWindow() {
        GLFWmonitor* monitor = nullptr;
        if (specs.fullscreen) {
            monitor = glfwGetPrimaryMonitor();
            const GLFWvidmode* mode = glfwGetVideoMode(monitor);
            specs.width = mode->width;
            specs.height = mode->height;
        }

        window = glfwCreateWindow(specs.width, specs.height,
                                specs.title.c_str(), monitor, nullptr);

        if (!window) {
            std::cerr << "Failed to create GLFW window\n";
            return false;
        }

        return true;
    }

    void Window::setupCallbacks() {
        glfwSetWindowUserPointer(window, this);

        bool isRawMouseMotionSupported = glfwRawMouseMotionSupported();
        if (isRawMouseMotionSupported) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

        glfwSetWindowCloseCallback(window, [](GLFWwindow* window) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            instance->getEventCallback()(event);
        });

        glfwSetWindowIconifyCallback(window, [](GLFWwindow* window, int32_t iconified) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowMinimizeEvent event((bool) iconified);
            instance->getEventCallback()(event);
        });

        glfwSetWindowMaximizeCallback(window, [](GLFWwindow* window, int32_t maximized) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowMaximizeEvent event((bool) maximized);
            instance->getEventCallback()(event);
        });

        glfwSetWindowSizeCallback(window, [](GLFWwindow* window, int32_t width, int32_t height) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            instance->specs.width = width, instance->specs.height = height;
            WindowResizeEvent event((uint32_t) width, (uint32_t) height);
            instance->getEventCallback()(event);
        });

        glfwSetWindowPosCallback(window, [](GLFWwindow* window, int32_t x, int32_t y) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowMoveEvent event(static_cast<uint32_t>(x), static_cast<uint32_t>(y));
            instance->getEventCallback()(event);
        });

        glfwSetWindowFocusCallback(window, [](GLFWwindow* window, int32_t focused) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            WindowFocusEvent event(static_cast<bool>(focused));
            instance->getEventCallback()(event);
        });

        glfwSetKeyCallback(window, [](GLFWwindow* window, int32_t key, int32_t scancode, int32_t action, int32_t mods) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    KeyPressEvent event((KeyCode) key);
                    instance->getEventCallback()(event);
                    break;
                } case GLFW_RELEASE: {
                    KeyReleaseEvent event((KeyCode) key);
                    instance->getEventCallback()(event);
                    break;
                } case GLFW_REPEAT: {
                    KeyHoldEvent event((KeyCode) key);
                    instance->getEventCallback()(event);
                    break;
                }
            }
        });

        glfwSetCharCallback(window, [](GLFWwindow* window, uint32_t codepoint) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            KeyTypeEvent event((KeyCode) codepoint);
            instance->getEventCallback()(event);
        });

        glfwSetCursorPosCallback(window, [](GLFWwindow* window, double x, double y) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            MouseMoveEvent event((float) x, (float) y);
            instance->getEventCallback()(event);
        });

        glfwSetScrollCallback(window, [](GLFWwindow* window, double xOffset, double yOffset) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));
            MouseScrollEvent event((float) xOffset, (float) yOffset);
            instance->getEventCallback()(event);
        });

        glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int32_t button, int32_t action, int32_t mods) {
            Window* instance = static_cast<Window*>(glfwGetWindowUserPointer(window));

            switch (action) {
                case GLFW_PRESS: {
                    MouseButtonPressEvent event((MouseButton) button);
                    instance->getEventCallback()(event);
                    break;
                } case GLFW_RELEASE: {
                    MouseButtonReleaseEvent event((MouseButton) button);
                    instance->getEventCallback()(event);
                    break;
                }
            }
        });

        // Add more callbacks as needed...
    }

    void Window::setupIcon() {
        if (!specs.iconPath.empty()) {
            GLFWimage icon;
            int channels;
            icon.pixels = stbi_load(specs.iconPath.string().c_str(),
                                  &icon.width, &icon.height, &channels, 4);
            if (icon.pixels) {
                glfwSetWindowIcon(window, 1, &icon);
                stbi_image_free(icon.pixels);
            }
        }
    }
    Window::EventCallbackFunc Window::getEventCallback() {
        return this->eventCallback;
    }

    void Window::setVsync(bool enabled) {
        vsyncEnabled = enabled;
        glfwSwapInterval(enabled ? 1 : 0);
    }

    void Window::maximise() {
        glfwMaximizeWindow(window);
    }

    void Window::center() {
        if (!window) return;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        if (!monitor) return;

        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        if (!mode) return;

        int monitorX, monitorY, monitorWidth, monitorHeight;
        glfwGetMonitorWorkarea(monitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);

        int windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);

        glfwSetWindowPos(window,
                         monitorX + (monitorWidth - windowWidth) / 2,
                         monitorY + (monitorHeight - windowHeight) / 2);
    }

    void Window::processEvents() {
        glfwPollEvents();
    }

    void Window::swapBuffers() {
        glfwSwapBuffers(window);
    }

    void Window::setEventCallback(const EventCallbackFunc& callback) {
        eventCallback = callback;
    }

    Window* Window::create(const WindowSpecs& specs) {
        return new Window(specs);
    }

    void Window::terminate() {
        if (window) {
            glfwDestroyWindow(window);
            window = nullptr;
        }
    }

    glm::vec2 Window::getSize() {
        glfwGetWindowSize(window, &specs.width, &specs.height);
        return { (float) specs.width, (float) specs.height };
    }

}