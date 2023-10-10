#pragma once
#include "Entity.h"

namespace Coconut {
using Group_t = std::size_t;

// Manager manage all entities
class Manager {
private:
    std::vector<std::unique_ptr<Entity>>        m_entities;
    std::array<std::vector<Entity*>, maxGroups> m_groupedEntities;

public:
    void update() {
        for(auto& e : m_entities)
            e->update();
    }

    void draw() {
        for(auto& e : m_entities)
            e->draw();
    }

    void refresh() {
        for(std::size_t i; i < maxGroups; i++) {
            auto& current_entity(m_groupedEntities[i]);
            current_entity.erase(std::remove_if(std::begin(current_entity), std::end(current_entity), [i](Entity* et) { return !et->isActive() || !et->hasGroup(i); }), std::end(current_entity));
        }
        // remove entity that is not active
        m_entities.erase(std::remove_if(std::begin(m_entities), std::end(m_entities), [](const std::unique_ptr<Entity>& mEntity) { return !mEntity->isActive(); }), std::end(m_entities));
    }

    void addToGroup(Entity* entity, Group_t group) {
        m_groupedEntities[group].emplace_back(entity);
    }

    std::vector<Entity*>& getGroup(Group_t group) {
        return m_groupedEntities[group];
    }
    // may be some uncleared stuff here...
    Entity& addEntity() {
        std::unique_ptr<Entity> uPtr = std::make_unique<Entity>();
        Entity&                 e    = *uPtr;
        m_entities.emplace_back(std::move(uPtr));
        return e;
    }
};
}  // namespace Coconut