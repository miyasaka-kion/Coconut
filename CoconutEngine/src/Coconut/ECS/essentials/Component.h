#pragma once

#include <cstddef>
#include <type_traits>

#include "Coconut/ECS/essentials/Entity.h"

namespace Coconut {
class Component;
using ComponentID_t = std::size_t;

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

}  // namespace Coconut