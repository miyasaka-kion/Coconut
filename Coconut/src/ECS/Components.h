#pragma once


#include <cassert>
#include <memory>
#include <filesystem>
#include <stdexcept>
#include <string_view>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL_render.h>
#include <box2d/box2d.h>
#include <entt.hpp>

#include "Core/Assert.h"
#include "Core/Camera.h"
#include "Core/Log.h"
#include "ECS/Entity.h"
#include "Render/SpriteLoader.h"
#include "Util/sdl_delete.h"

// This class will give different render resources provided different state of that Fixture.
// There is only one state by default.
class SpriteComponent {
public:
    SpriteComponent() = delete;
    SpriteComponent(const SpriteInfo& sprite_info) : m_sprite_infos{ sprite_info }, m_ind(0) {
        CC_CORE_WARN("The size of the render box is not set. This overload will be removed in future version.");
        // this is a temp solution.
        // this ctor may be removed in future version.
    }
    SpriteComponent(const SpriteInfo& sprite_info, const b2Vec2& box_size) : m_sprite_infos{ sprite_info }, m_ind(0), m_box_size{ box_size } {
        CC_CORE_WARN("The offset of the render box is not set. This overload will be removed in future version.");
        // this is a temp solution.
        // this ctor may be removed in future version.
    }

    SpriteComponent(const SpriteInfo& sprite_info, const b2Vec2& box_size, const b2Vec2& local_offset) : m_sprite_infos{ sprite_info }, m_ind(0), m_box_size{box_size}, m_local_offset{local_offset}  {}

    [[nodiscard]] const SDL_Texture* GetTexture() const {
        CC_ASSERT(!m_sprite_infos.empty(), "_INFO_: internal member: m_sprite_infos should not be empty");
        return m_sprite_infos[m_ind].texture;
    }

    [[nodiscard]] SDL_Rect GetSrcRect() const {
        CC_ASSERT(!m_sprite_infos.empty(), "_INFO_:internal member: m_sprite_infos should not be empty!");
        return m_sprite_infos[m_ind].rect;
    }

    [[nodiscard]] SpriteInfo& GetSpriteInfo() {
        CC_ASSERT(!m_sprite_infos.empty(), "_INFO_:internal member: m_sprite_infos should not be empty!");
        return m_sprite_infos[m_ind];
    }

    void AddSpriteInfo(const SpriteInfo& info) {
        m_sprite_infos.push_back(info);
    }

    virtual void UpdateRenderCandidate([[maybe_unused]] std::size_t ind) {
        CC_ASSERT(ind <= m_sprite_infos.size(), "_INFO_: index out of range!");
        m_ind = ind;
    }

    void SetBoxSize(const b2Vec2& box_size) { m_box_size = box_size; }
    
    b2Vec2 GetBoxSize() const  { return m_box_size; }

    void SetOffset(const b2Vec2& offset) { m_local_offset = offset; }

    b2Vec2 GetLocalOffset() const { 
        return m_local_offset; 
    }


protected:
    void SetState(std::size_t ind) {
        CC_ASSERT(ind < m_sprite_infos.size() && ind >= 0, "_INFO_: index out of range!");
        m_ind = ind;
    }

private:
    // SDL_Renderer* m_renderer; // is this necessary?
    std::vector< SpriteInfo > m_sprite_infos;
    std::size_t               m_ind;

    std::string m_sprite_name;
    std::filesystem::path m_texture_path;

    // Entity entity; TODO

    // render info
    b2Vec2 m_box_size = b2Vec2(5.0f, 5.0f);
    b2Vec2 m_local_offset = b2Vec2_zero;
};

struct PhysicsComponent {
    PhysicsComponent() = default;

    PhysicsComponent(b2Body* body) : m_body(body) {
        CC_ASSERT(body, "_INFO_: body must not be nullptr");
    }

    ~PhysicsComponent() {
        CC_CORE_INFO("PhysicsComponent: body [{}] destroyed", static_cast<void*>(m_body));
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

struct DebugLayerComponent {
    DebugLayerComponent(std::unique_ptr<Layer> layer)  {
        m_layer = (std::move(layer));
    }

    void GuiRender() {
        m_layer->GuiRender();
    }

    std::unique_ptr< Layer > m_layer;
};
