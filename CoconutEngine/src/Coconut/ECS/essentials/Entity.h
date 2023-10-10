#pragma once

#include <bitset>
#include <memory>
#include <vector>

#include "Coconut/ECS/essentials/Component.h"
#include "Coconut/ECS/essentials/Manager.h"

namespace Coconut {

constexpr std::size_t maxComponents = 32;
constexpr std::size_t maxGroups     = 32;

using ComponentBitset_t = std::bitset<maxComponents>;
using ComponentArray_t  = std::array<Component*, maxComponents>;
using GroupBitset_t     = std::bitset<maxGroups>;

class Entity {
private:
    Coconut::Manager&                       m_manager;  // Point to the parent level: Manager
    bool                                    m_active = true;
    std::vector<std::unique_ptr<Component>> m_components;  // An Entity has a set of components

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

}  // namespace Coconut