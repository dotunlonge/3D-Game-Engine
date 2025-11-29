#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

Transform::Transform() : m_Position(0.0f), m_Rotation(1.0f, 0.0f, 0.0f, 0.0f), m_Scale(1.0f) {
}

glm::mat4 Transform::GetMatrix() const {
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Position);
    glm::mat4 rotation = glm::mat4_cast(m_Rotation);
    glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_Scale);
    return translation * rotation * scale;
}

glm::mat4 Transform::GetInverseMatrix() const {
    return glm::inverse(GetMatrix());
}

