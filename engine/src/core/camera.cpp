//
// Created by admin on 19/01/2025.
//

#include <algorithm>
#include <core/camera.h>
#include <core/input.h>

namespace Core::Engine {

void Camera::ProcessKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    switch (direction) {
        case CameraMovement::FORWARD:
            Position += Front * velocity;
            break;
        case CameraMovement::BACKWARD:
            Position -= Front * velocity;
            break;
        case CameraMovement::LEFT:
            Position -= Right * velocity;
            break;
        case CameraMovement::RIGHT:
            Position += Right * velocity;
            break;
        case CameraMovement::UP:
            Position += Up * velocity;
            break;
        case CameraMovement::DOWN:
            Position -= Up * velocity;
            break;
    }
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Prevent flipping by constraining pitch
    if (constrainPitch) {
        Pitch = std::clamp(Pitch, -89.0f, 89.0f);
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;
    Zoom = std::clamp(Zoom, 1.0f, 45.0f);
}

// Add this to the header file
    void Camera::Update(float deltaTime) {
    // Process keyboard input
    if (Input::IsKeyHeld(KeyCode::W))
        ProcessKeyboard(CameraMovement::FORWARD, deltaTime);
    if (Input::IsKeyHeld(KeyCode::S))
        ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);
    if (Input::IsKeyHeld(KeyCode::A))
        ProcessKeyboard(CameraMovement::LEFT, deltaTime);
    if (Input::IsKeyHeld(KeyCode::D))
        ProcessKeyboard(CameraMovement::RIGHT, deltaTime);
    if (Input::IsKeyHeld(KeyCode::E))
        ProcessKeyboard(CameraMovement::UP, deltaTime);
    if (Input::IsKeyHeld(KeyCode::Q))
        ProcessKeyboard(CameraMovement::DOWN, deltaTime);

    // Process mouse movement
    glm::vec2 mouseDelta = Input::GetMousePositionDelta();
    if (Input::IsMouseButtonHeld(MouseButton::Right)) {  // Only rotate when right mouse button is held
        ProcessMouseMovement(mouseDelta.x, -mouseDelta.y, true);  // Invert Y for intuitive control
    }

    // Process mouse scroll
    glm::vec2 scrollDelta = Engine::Input::GetMouseScrollDelta();
    ProcessMouseScroll(scrollDelta.y);
}

} // namespace Core::Engine