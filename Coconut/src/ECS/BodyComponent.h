#pragma once

#include <box2d/box2d.h>

struct BodyComponent {
    BodyComponent() = default;
    BodyComponent(b2Body* body) : m_body(body) {}

    ~BodyComponent() {
        m_body->GetWorld()->DestroyBody(m_body);
    }
    
    
    [[nodiscard]] const b2Vec2& GetPosition() const {
        return m_body->GetPosition();
    }

    [[nodiscard]] const float GetAngle() const {
        return m_body->GetAngle();
    }
    b2Body* m_body;
};
