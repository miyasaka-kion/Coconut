#pragma once

#include <entt.hpp>

#include "Core/Assert.h"
#include "Core/GameContext.h"
#include "ECS/SpriteComponent.h"

class Entity {
public:
    Entity() = delete;
    Entity(GameContext* context, entt::entity entity) : m_gameContext(context), m_entity_handle(entity) {}
    virtual ~Entity() = default;

    template < typename T >
    bool HasComponent() {
        return m_gameContext->m_reg.all_of< T >(m_entity_handle);
    }

    template < typename T, typename... Args >
    T& AddComponent(Args&&... args) {
        CC_ASSERT((!HasComponent< T >()), "Entity already has component!");
        return m_gameContext->m_reg.emplace< T >(m_entity_handle, std::forward< Args >(args)...);
    }

    template < typename T >
    T& GetComponent() {
        CC_ASSERT(HasComponent< T >(), "Entity does not have component!");
        return m_gameContext->m_reg.get< T >(m_entity_handle);
    }

    template < typename T >
    void RemoveComponent() {
        CC_ASSERT(HasComponent< T >(), "Entity does not have component!");
        m_gameContext->m_reg.remove< T >(m_entity_handle);
    }

    /**
     * A templated function that checks if all components specified by Args are present for the current entity.
     *
     * @tparam Args variadic template parameters for component types to check
     *
     * @return true if all components are present, false otherwise
     */
    template < typename... Args >
    bool AllOf() {
        return m_gameContext->m_reg.all_of< Args... >(m_entity_handle);
    }

    operator bool() const {
        return m_entity_handle != entt::null;
    }
    operator entt::entity() const {
        return m_entity_handle;
    }
    operator uint32_t() const {
        return static_cast< uint32_t >(m_entity_handle);
    }

    bool operator==(const Entity& other) const {
        return m_entity_handle == other.m_entity_handle && m_gameContext == other.m_gameContext;
    }

    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

private:
    GameContext* m_gameContext;
    entt::entity m_entity_handle;
};
