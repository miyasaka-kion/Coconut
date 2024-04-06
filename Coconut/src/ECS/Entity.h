#pragma once

#include <entt.hpp>

#include "Core/Assert.h"
#include "Core/GameContext.h"
#include "ECS/SpriteComponent.h"

class Entity {
public:
    Entity() = delete;
    Entity(GameContext* context, entt::entity entity) : m_gameContext(context), m_entityHandle(entity) {}
    virtual ~Entity() = default;

    template < typename T >
    bool HasComponent() {
        return m_gameContext->m_reg.all_of< T >(m_entityHandle);
    }

    template < typename T, typename... Args >
    T& AddComponent(Args&&... args) {
        CC_ASSERT((!HasComponent< T >()), "Entity already has component!");
        return m_gameContext->m_reg.emplace< T >(m_entityHandle, std::forward< Args >(args)...);
    }

    template < typename T >
    [[nodiscard]] decltype(auto) GetComponent() { //?? TODO!!!
        CC_ASSERT(HasComponent< T >(), "Entity does not have component!");
        return m_gameContext->m_reg.get< T >(m_entityHandle);
    }

    template < typename T >
    void RemoveComponent() {
        CC_ASSERT(HasComponent< T >(), "Entity does not have component!");
        m_gameContext->m_reg.remove< T >(m_entityHandle);
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
        return m_gameContext->m_reg.all_of< Args... >(m_entityHandle);
    }

    operator bool() const {
        return m_entityHandle != entt::null;
    }
    operator entt::entity() const {
        return m_entityHandle;
    }
    operator uint32_t() const {
        return static_cast< uint32_t >(m_entityHandle);
    }

    bool operator==(const Entity& other) const {
        return m_entityHandle == other.m_entityHandle && m_gameContext == other.m_gameContext;
    }

    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

    GameContext* GetContext() const {
        return m_gameContext;
    }

private:
    GameContext* m_gameContext;
    entt::entity m_entityHandle;
};
