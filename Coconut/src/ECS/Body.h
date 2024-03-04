#pragma once

#include "ECS/Entity.h"
#include <box2d/box2d.h>

class Body : public Component {
public:
    Body() = delete;
    Body(b2Body* body) : m_body(body){};

    b2Body* GetBody() const {
        return m_body;
    }

private:
    b2Body* m_body;
};