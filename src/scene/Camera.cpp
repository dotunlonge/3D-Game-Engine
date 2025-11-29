#include "Camera.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <algorithm>

Camera::Camera(glm::vec3 position, float fov, float aspect) : m_FOV(fov), m_Aspect(aspect) {
    m_Transform.SetPosition(position);
    UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const {
    glm::vec3 pos = m_Transform.GetPosition();
    glm::vec3 forward = GetForward();
    glm::vec3 up = GetUp();
    return glm::lookAt(pos, pos + forward, up);
}

glm::mat4 Camera::GetProjectionMatrix() const {
    return glm::perspective(glm::radians(m_FOV), m_Aspect, m_Near, m_Far);
}

glm::mat4 Camera::GetViewProjectionMatrix() const {
    return GetProjectionMatrix() * GetViewMatrix();
}

glm::vec3 Camera::GetForward() const {
    float yawRad = glm::radians(m_Yaw);
    float pitchRad = glm::radians(m_Pitch);
    return glm::normalize(glm::vec3(
        cos(yawRad) * cos(pitchRad),
        sin(pitchRad),
        sin(yawRad) * cos(pitchRad)
    ));
}

glm::vec3 Camera::GetRight() const {
    return glm::normalize(glm::cross(GetForward(), glm::vec3(0.0f, 1.0f, 0.0f)));
}

glm::vec3 Camera::GetUp() const {
    return glm::normalize(glm::cross(GetRight(), GetForward()));
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = m_Speed * deltaTime;
    glm::vec3 forward = GetForward();
    glm::vec3 right = GetRight();

    if (direction == 0) // Forward
        m_Transform.Translate(forward * velocity);
    if (direction == 1) // Backward
        m_Transform.Translate(-forward * velocity);
    if (direction == 2) // Left
        m_Transform.Translate(-right * velocity);
    if (direction == 3) // Right
        m_Transform.Translate(right * velocity);
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= m_Sensitivity;
    yoffset *= m_Sensitivity;

    m_Yaw += xoffset;
    m_Pitch -= yoffset; // Reversed since y-coordinates go from bottom to top

    if (constrainPitch) {
        if (m_Pitch > 89.0f)
            m_Pitch = 89.0f;
        if (m_Pitch < -89.0f)
            m_Pitch = -89.0f;
    }

    UpdateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    m_FOV -= (float)yoffset;
    if (m_FOV < 1.0f)
        m_FOV = 1.0f;
    if (m_FOV > 45.0f)
        m_FOV = 45.0f;
}

void Camera::UpdateCameraVectors() {
    // Camera vectors updated via GetForward/GetRight/GetUp
}

