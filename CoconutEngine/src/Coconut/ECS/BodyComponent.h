#pragma once

#include <stdexcept>

#include <box2d/box2d.h>

#include "Coconut/ECS/ECS.h"
#include "Coconut/Log.h"

namespace Coconut {
class BodyComponent : public Coconut::Component {
public:
    BodyComponent(b2World* world) : m_world(world) {}
    BodyComponent(b2World* world, b2BodyDef& body_def) : m_world(world) {
        m_body = m_world->CreateBody(&body_def);
    }

    
    b2Vec2 getPosition() {
        return m_body->GetPosition();
    }

    float getAngle() {
        return m_body->GetAngle();
    }
private:
    b2Body*  m_body;
    b2World* m_world;
};
}  // namespace Coconut