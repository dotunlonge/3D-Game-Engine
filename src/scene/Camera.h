#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Transform.h"

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), float fov = 45.0f, float aspect = 16.0f/9.0f);

    glm::mat4 GetViewMatrix() const;
    glm::mat4 GetProjectionMatrix() const;
    glm::mat4 GetViewProjectionMatrix() const;

    void SetPosition(const glm::vec3& position) { m_Transform.SetPosition(position); }
    void SetRotation(const glm::quat& rotation) { m_Transform.SetRotation(rotation); }

    glm::vec3 GetPosition() const { return m_Transform.GetPosition(); }
    glm::vec3 GetForward() const;
    glm::vec3 GetRight() const;
    glm::vec3 GetUp() const;

    void ProcessKeyboard(int direction, float deltaTime);
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    void ProcessMouseScroll(float yoffset);

    float GetFOV() const { return m_FOV; }
    void SetAspect(float aspect) { m_Aspect = aspect; }

private:
    void UpdateCameraVectors();

    Transform m_Transform;
    float m_FOV;
    float m_Aspect;
    float m_Near = 0.1f;
    float m_Far = 1000.0f;

    float m_Yaw = -90.0f;
    float m_Pitch = 0.0f;
    float m_Speed = 2.5f;
    float m_Sensitivity = 0.1f;
    float m_Zoom = 45.0f;
};

