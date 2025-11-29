#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Transform {
public:
    Transform();

    glm::mat4 GetMatrix() const;
    glm::mat4 GetInverseMatrix() const;

    void SetPosition(const glm::vec3& position) { m_Position = position; }
    void SetRotation(const glm::quat& rotation) { m_Rotation = rotation; }
    void SetScale(const glm::vec3& scale) { m_Scale = scale; }

    glm::vec3 GetPosition() const { return m_Position; }
    glm::quat GetRotation() const { return m_Rotation; }
    glm::vec3 GetScale() const { return m_Scale; }

    void Translate(const glm::vec3& translation) { m_Position += translation; }
    void Rotate(const glm::quat& rotation) { m_Rotation = rotation * m_Rotation; }

private:
    glm::vec3 m_Position;
    glm::quat m_Rotation;
    glm::vec3 m_Scale;
};

