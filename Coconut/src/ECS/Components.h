#pragma once

#include "ECS/SpriteComponent.h"

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

struct HealthComponent {
    void TakeDamage(float damage) {
        m_health -= damage;
    }

    bool IsDead() {
        return m_health <= 0.0f;
    }

    void AddHealth(float increase) {
        m_health += increase;
        m_health = std::min(m_health, 100.0f);
    }

    [[nodiscard]] const float GetHealth() const {
        return m_health;
    }
private:
    float m_health = 100.0f;
};

struct PlayerComponent {

};
