#pragma once

#include <cmath>
#include <tuple>

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include <box2d/box2d.h>

#include "Coconut/ConstantSpec.h"
#include "Coconut/ECS/ECS.h"
#include "Coconut/Log.h"
#include "Coconut/MetricConverter.h"
#include "Coconut/ECS/BodyComponent.h"

namespace Coconut {
// TransformComponent is a "reference" to BodyComponent, each time it updates its info following the BodyComponent
class TransformComponent : public Coconut::Component {
public:
    float scale = Coconut::Constants::defaultScale;

public:
    TransformComponent(b2Vec2 position, float angle) : m_active(true), m_position(position), m_angle(angle) {
        if (entity->hasComponent<Coconut::BodyComponent>() ) {
            CC_CORE_WARN("Adding TransformComponent to this entity will make the TransformComponent in active.");
            m_active = false;
        }
    }

    void filter() {
        throw std::logic_error("calling an inactive Transform Component!");
    }

    b2Vec2 getPosition() {
        filter();
        return m_position;
    }

    float getAngle() {
        filter();
        return m_angle;
    }

    void init() override {
        CC_CORE_INFO("TransformComponent initialized! Positiion = ({}, {}), rotation = {}", getPosition().x, getPosition().y, getAngle());
    }

    void update() override {
        
    }

    void setPositionAngle(b2Vec2 position, float angle) {
        
        m_position = position;
        m_angle = angle;
    }

private:
    b2Vec2 m_position;
    float m_angle;
    bool m_active;
};

}  // namespace Coconut