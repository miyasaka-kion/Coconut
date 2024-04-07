#pragma once

#include "Core/Assert.h"
#include "Core/Log.h"
#include "ECS/SpriteComponent.h"
#include <memory>

struct PhysicsComponent {
    PhysicsComponent() = default;

    PhysicsComponent(b2Body* body) : m_body(body) {
        CC_ASSERT(body, "_INFO_: body must not be nullptr");
    }

    ~PhysicsComponent() {
        CC_CORE_INFO("PhysicsComponent: body destroyed");
        m_body->GetWorld()->DestroyBody(m_body);
    }

    [[nodiscard]]  b2Body* GetBody() const {
        return m_body;
    }

    [[nodiscard]] b2Fixture* GetFixtureList() {
        return m_body->GetFixtureList();
    }

    [[nodiscard]] const b2Vec2& GetPosition() const {
        return m_body->GetPosition();
    }

    [[nodiscard]] const float GetAngle() const {
        return m_body->GetAngle();
    }
    b2Body* m_body;
};

struct TagComponent {
    std::string Tag;

    TagComponent()                    = default;
    TagComponent(const TagComponent&) = default;
    TagComponent(const std::string& tag) : Tag(tag) {}
};

class ScriptableEntity;

struct NativeScriptComponent
{
    // ScriptableEntity* Instance = nullptr;

    // ScriptableEntity*(*InstantiateScript)();
    // void (*DestroyScript)(NativeScriptComponent*);

    // template<typename T>
    // void Bind()
    // {
    //     InstantiateScript = []() { return static_cast<ScriptableEntity*>(new T()); };
    //     DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
    // }
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
    bool is_camera_following = false;
};

// template < typename LayerType, typename... Args >
// struct DebugLayerComponent {
//     static_assert(std::is_base_of< Layer, LayerType >::value, "LayerType must be a subclass of Layer");
//     DebugLayerComponent(Args&&... args) : m_layer(std::make_unique< LayerType >(std::forward< Args >(args)...)) {}

//     void GuiRender() {
//         m_layer->GuiRender();
//     }

//     std::unique_ptr< LayerType > m_layer;
// };

struct DebugLayerComponent {
    DebugLayerComponent(std::unique_ptr<Layer> layer)  {
        m_layer = (std::move(layer));
    }

    void GuiRender() {
        m_layer->GuiRender();
    }

    std::unique_ptr< Layer > m_layer;
};
