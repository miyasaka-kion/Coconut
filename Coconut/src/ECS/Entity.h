#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <memory>
#include <vector>


#include <box2d/box2d.h>

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
template < typename T >
inline ComponentID_t GetComponentTypeID() noexcept {
    static ComponentID_t typeID = GetComponentTypeID();
    return typeID;
}

constexpr std::size_t c_max_components = 32;

using ComponentBitset_t = std::bitset< c_max_components >;
using ComponentArray_t  = std::array< Component*, c_max_components >;

class Component {
public:
    virtual void Init() {}
    virtual void Update() {}
    virtual void Render() {}

    virtual ~Component() {}

    Entity* m_entity;
};

// Entity has many components
// Entity defined by user should inherit the Entity class
// Note: The member functions cannot be called in the constructor of Component!
class Entity {
public:
    // Entity() = delete;
    // Entity(b2Body* body) : m_body(body) {} // TODO: make this private

    // virtual ~Entity() {}
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

    template < typename T >
    bool HasComponent() const {
        return m_componentBitset[GetComponentTypeID< T >()];
    }

    // TODO: Add Mechanism to make component can only be constructed in this way
    template < typename T, typename... TArgs >
    T& AddComponent(TArgs&&... mArgs) {

        // TODO: Add restriction to T to make sure it is a subclass of Component
        std::unique_ptr< T > uPtr = std::make_unique< T >(std::forward< TArgs >(mArgs)...);
        uPtr->m_entity              = this;

        T* rawPtr = uPtr.get(); 
        m_components.emplace_back(std::move(uPtr));

        m_componentArray[GetComponentTypeID< T >()]  = rawPtr;
        m_componentBitset[GetComponentTypeID< T >()] = true;

        rawPtr->Init();
        return *rawPtr;
    }

    template < typename T >
    T& GetComponent() const {
        auto ptr(m_componentArray[GetComponentTypeID< T >()]);
        return *static_cast< T* >(ptr);
    }

private:
    bool                                        m_active = true;
    std::vector< std::unique_ptr< Component > > m_components;

    ComponentArray_t  m_componentArray;
    ComponentBitset_t m_componentBitset;

    // b2Body* m_body;
};

class EntityManager {
    // // TODO: add a private constructor ?
    // public:
    //     EntityManager() = delete;
    //     EntityManager(b2World* world) {
    //         m_world = world;
    //     }

public:
    void Update() {
        for(auto& e : m_entities)
            e->Update();
    }

    void Render() {
        for(auto& e : m_entities)
            e->RenderComponents();
    }

    void RemoveInactive() {
        m_entities.erase(std::remove_if(std::begin(m_entities), std::end(m_entities), [](const std::unique_ptr< Entity >& mEntity) { return !mEntity->IsActive(); }), std::end(m_entities));

        // remember to clean up body in the subclass of Entity
    }

    Entity* AddEntity() {
        auto    uPtr = std::make_unique< Entity >();
        m_entities.emplace_back(std::move(uPtr));
        
        return m_entities.back().get();
    }

    void ClearEntities() {
        m_entities.clear();
    }

private:
    std::vector< std::unique_ptr< Entity > > m_entities;
    // b2World*                                 m_world;
};
