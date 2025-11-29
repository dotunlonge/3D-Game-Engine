#include "Scene.h"
#include <algorithm>

Scene::Scene() {
}

Scene::~Scene() {
}

Entity* Scene::CreateEntity() {
    EntityID id = m_NextEntityID++;
    m_Entities.push_back(std::make_unique<Entity>(id));
    return m_Entities.back().get();
}

void Scene::RemoveEntity(EntityID id) {
    m_Entities.erase(
        std::remove_if(m_Entities.begin(), m_Entities.end(),
            [id](const std::unique_ptr<Entity>& e) { return e->GetID() == id; }),
        m_Entities.end()
    );
}

Entity* Scene::GetEntity(EntityID id) {
    for (auto& entity : m_Entities) {
        if (entity->GetID() == id) {
            return entity.get();
        }
    }
    return nullptr;
}

void Scene::Update(float deltaTime) {
    // Scene update logic
}

void Scene::Render() {
    // Scene rendering logic
}

