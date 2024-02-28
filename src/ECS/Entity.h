#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <memory>
#include <vector>

#include "Log.h"

class Component;
class Entity;
using ComponentID_t = std::size_t;

// this function shouldn't be called externally,
// this should be called for getComponentTypeID<blah>() only!
// maybeencapsulate needed in later version
inline ComponentID_t GetComponentTypeID() {
    static ComponentID_t lastID = 0;
    return lastID++;
}

// get component ID for a specific type
// each type has it's own, separate type counter
// should only call this function
template <typename T> inline ComponentID_t GetComponentTypeID() noexcept {
    static ComponentID_t typeID = GetComponentTypeID();
    return typeID;
}

constexpr std::size_t c_max_components = 32;

using ComponentBitset_t = std::bitset<c_max_components>;
using ComponentArray_t  = std::array<Component*, c_max_components>;

class Component {
public:
    Entity* entity;

    virtual void Init() {}
    virtual void Update() {}
    virtual void Render() {}

    virtual ~Component() {}
};

// Entity has many components
// Entity defined by user should inherit the Entity class
// Note: The member functions cannot be called in the constructor of Component!
class Entity {
private:
    bool                                    m_active = true;
    std::vector<std::unique_ptr<Component>> m_components;

    ComponentArray_t  m_componentArray;
    ComponentBitset_t m_componentBitset;

public:
    void Update() {
        for(auto& c : m_components)
            c->Update();
    }
    void RenderComponents() {
        for(auto& c : m_components)
            c->Render();
    }

    void Destroy() {
        m_active = false;
    }

    bool IsActive() const {
        return m_active;
    }

    template <typename T> bool HasComponent() const {
        return m_componentBitset[GetComponentTypeID<T>()];
    }

    // TODO: Add Mechanism to make component can only be constructed in this way (?)
    template <typename T, typename... TArgs> T& AddComponent(TArgs&&... mArgs) {
        std::unique_ptr<T> uPtr = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
        uPtr->entity            = this;

        T* rawPtr = uPtr.get();

        m_components.emplace_back(std::move(uPtr));

        m_componentArray[GetComponentTypeID<T>()]  = rawPtr;
        m_componentBitset[GetComponentTypeID<T>()] = true;

        rawPtr->init();
        return *rawPtr;
    }

    template <typename T> T& getComponent() const {
        auto ptr(m_componentArray[GetComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
};

class EntityManager {
    // TODO: add a private constructor ?

public:
    void Update() {
        for(auto& e : m_entities)
            e->Update();
    }

    void RenderEntities() {
        for(auto& e : m_entities)
            e->RenderComponents();
    }

    void RemoveInactive() {
        m_entities.erase(std::remove_if(std::begin(m_entities), std::end(m_entities), [](const std::unique_ptr<Entity>& mEntity) { return !mEntity->IsActive(); }), std::end(m_entities));
    }

    // may be some uncleared stuff here...
    Entity& AddEntity() {
        auto uPtr = std::make_unique<Entity>();
        Entity&                 e    = *uPtr;
        m_entities.emplace_back(std::move(uPtr));
        return e;
    }

    private:
    std::vector<std::unique_ptr<Entity>> m_entities;

};

