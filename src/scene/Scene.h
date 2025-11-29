#pragma once

#include "Entity.h"
#include <vector>
#include <memory>

class Scene {
public:
    Scene();
    ~Scene();

    Entity* CreateEntity();
    void RemoveEntity(EntityID id);
    Entity* GetEntity(EntityID id);

    void Update(float deltaTime);
    void Render();

private:
    std::vector<std::unique_ptr<Entity>> m_Entities;
    EntityID m_NextEntityID = 1;
};

