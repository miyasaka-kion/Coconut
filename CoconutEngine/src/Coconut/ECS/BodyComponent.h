#pragma once

#include "Coconut/ECS/ECS.h"
#include <box2d/box2d.h>

namespace Coconut {
class BodyComponent : public Coconut::Component {
public:
    BodyComponent(b2World* world) : m_world(world) {}
    BodyComponent(const b2BodyDef* body_def, b2World* world) : m_world(world){
        m_body = m_world->CreateBody(body_def);
    }
    
private:
    b2Body*  m_body;
    b2World* m_world;
};
}  // namespace Coconut