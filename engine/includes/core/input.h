#ifndef ENGINE_INPUT_H
#define ENGINE_INPUT_H

#include "keycodes.h"
#include "event/event.h"
#include <GLFW/glfw3.h>
#include <sys/stat.h>


namespace Core::Engine {
        extern double scrollX, scrollY;
        void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);


    class Input {
    private:
        inline static GLFWwindow* windowHandle = nullptr;
        inline static glm::vec2 mousePosition, lastMousePosition, deltaMousePosition;
        inline static glm::vec2 mouseScroll, deltaMouseScroll;

    public:
        static void Initialize(GLFWwindow* windowHandle);
        static void Terminate();

        static void HandleEvent(Event& event);

        static KeyState GetKeyState(KeyCode keyCode);
        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsKeyHeld(KeyCode keyCode);
        static bool IsKeyReleased(KeyCode keyCode);

        static ButtonState GetMouseButtonState(MouseButton button);
        static bool IsMouseButtonPressed(MouseButton button);
        static bool IsMouseButtonHeld(MouseButton button);
        static bool IsMouseButtonReleased(MouseButton button);

        static glm::vec2 GetMouseScrollDelta();
        static glm::vec2 GetMouseScroll();
        static void SetMouseScroll(const glm::vec2& scroll);

        static glm::vec2 GetMousePositionDelta();
        static glm::vec2 GetMousePosition();
        static void SetMousePosition(const glm::vec2& position);

        static void SetCursorMode(CursorMode mode);
        static CursorMode GetCursorMode();
    };
} // namespace Core::Engine
#endif // ENGINE_INPUT_H