#pragma once

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

namespace Coconut {

class TransformComponent : public Coconut::Component {
public:
    b2Transform transform;

public:
    float scale = Coconut::Constants::defaultScale;

public:
    TransformComponent() {
        transform.SetIdentity();
    }

    TransformComponent(b2Vec2 position, float angle) {
        transform.Set(position, angle);
    }

    b2Vec2 getPosition() {
        return transform.p;
    }

    float getAngle() {
        return transform.q.GetAngle();
    }

    void init() override {
        CC_CORE_INFO("TransformComponent initialized! Positiion = ({}, {}), rotation = {}", getPosition().x, getPosition().y, getAngle());
    }

    void update() override {}

    void set(b2Vec2 position, float angle) {
        transform.Set(position, angle);
    }
};

}  // namespace Coconut