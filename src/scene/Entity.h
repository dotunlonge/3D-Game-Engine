#pragma once

#include "Transform.h"
#include <cstdint>

using EntityID = uint32_t;

class Entity {
public:
    Entity(EntityID id) : m_ID(id) {}

    EntityID GetID() const { return m_ID; }
    Transform& GetTransform() { return m_Transform; }
    const Transform& GetTransform() const { return m_Transform; }

private:
    EntityID m_ID;
    Transform m_Transform;
};

