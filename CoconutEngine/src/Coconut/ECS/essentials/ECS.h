#pragma once

#include <algorithm>
#include <array>
#include <bitset>
#include <memory>
#include <type_traits>
#include <vector>

#include "Coconut/Log.h"

namespace Coconut {

class Component;
class Entity;
class Manager;

using ComponentID_t = std::size_t;
using Group_t = std::size_t;

// this function shouldn't be called externally,
// this should be called for getComponentTypeID<blah>() only!
// maybeencapsulate needed in later version
inline ComponentID_t getComponentTypeID() {
    static ComponentID_t lastID = 0;
    return lastID++;
}

// get component ID for a specific type
// each type has it's own, separate type counter
// should only call this function
template <typename T> inline ComponentID_t getComponentTypeID() noexcept {
    static_assert(std::is_base_of<Coconut::Component, T>::value, "getComponentTypeID try to instantiate a non-supported type!");
    static ComponentID_t typeID = getComponentTypeID();
    return typeID;
}

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups     = 32;

using ComponentBitset_t = std::bitset<maxComponents>;
using ComponentArray_t  = std::array<Component*, maxComponents>;
using GroupBitset_t     = std::bitset<maxGroups>;

// Component
class Component {
public:
    // points to the entity it belongs to
    Entity* entity;

    virtual void init() {}
    virtual void update() {}
    virtual void draw() {}

    virtual ~Component() {}
};

// Entity has many components
class Entity {
private:
    Coconut::Manager&                       m_manager; // Point to the parent level: Manager
    bool                                    m_active = true;
    std::vector<std::unique_ptr<Component>> m_components; // An Entity has a set of components

    ComponentArray_t  m_componentArray;
    ComponentBitset_t m_componentBitset;
    GroupBitset_t     m_groupBitset;

public:
    Entity(Coconut::Manager& manager_p) : m_manager(manager_p) {}

    void update() {
        for(auto& c : m_components)
            c->update();
    }
    void draw() {
        for(auto& c : m_components)
            c->draw();
    }

    bool isActive() const {
        return m_active;
    }
    void destroy() {
        m_active = false;
    }

    bool hasGroup(Group_t group) {
        return m_groupBitset[group];
    }

    void addGroup(Group_t group) {
        m_groupBitset[group] = true;
        m_manager.addToGroup(this, group);
        // 为什么上面这一行报错了
    }

    void delGroup(Group_t group) {
    // TODO...
    }

    template <typename T> bool hasComponent() const {
        // CC_CORE_INFO("getComponentTypeID<T>() = {}", getComponentTypeID<T>());
        return m_componentBitset[getComponentTypeID<T>()];
    }

    template <typename T, typename... TArgs> T& addComponent(TArgs&&... mArgs) {
        std::unique_ptr<T> uPtr = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
        uPtr->entity            = this;

        T* rawPtr = uPtr.get();

        m_components.emplace_back(std::move(uPtr));

        m_componentArray[getComponentTypeID<T>()]  = rawPtr;
        m_componentBitset[getComponentTypeID<T>()] = true;

        rawPtr->init();
        return *rawPtr;
    }

    template <typename T> T& getComponent() const {
        auto ptr(m_componentArray[getComponentTypeID<T>()]);
        return *static_cast<T*>(ptr);
    }
};

// Manager manage all entities
class Manager {
private:
    std::vector<std::unique_ptr<Entity>> m_entities;
    std::array<std::vector<Entity* >, maxGroups> m_groupedEntities;
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
        for (std::size_t i; i < maxGroups; i++) {
            auto& current_entity(m_groupedEntities[i]);
            current_entity.erase(
                std::remove_if(
                    std::begin(current_entity),
                    std::end(current_entity),
                    [i](Entity* et) {
                        return !et->isActive() || !et->hasGroup(i);
                    }
                ), std::end(current_entity)
            );
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