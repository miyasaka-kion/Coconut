#pragma once

#include <entt.hpp>

#include "Core/GameContext.h"

namespace Coconut {

class Entity {
public:
    Entity()          = delete;
    Entity(GameContext* scene) : m_scene(scene) {}
    virtual ~Entity() = default;
    
private:
    GameContext* m_scene;
    entt::entity m_entity_handle;
};


class EntityManager {

};


}  // namespace Coconut