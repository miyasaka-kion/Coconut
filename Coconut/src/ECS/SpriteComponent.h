#pragma once

#include <cassert>
#include <filesystem>
#include <stdexcept>
#include <string_view>

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL_render.h>
#include <box2d/box2d.h>
#include <entt.hpp>

#include "Core/Assert.h"
#include "ECS/Entity.h"
#include "Render/SpriteLoader.h"
#include "Util/sdl_delete.h"

// This class will give different render resources provided different state of that Fixture.
// There is only one state by default.
class SpriteComponent {
public:
    SpriteComponent() = delete;
    // SpriteComponent(SDL_Renderer* renderer, std::string_view texture_name);
SpriteComponent(const SpriteInfo& sprite_info, b2Fixture* fixture) : m_sprite_infos{ sprite_info }, m_fixture{ fixture } {}

    [[nodiscard]] const SDL_Texture* GetTexture() const {
        CC_ASSERT(!m_sprite_infos.empty(), "_INFO_: internal member: m_sprite_infos should not be empty");
        return m_sprite_infos[m_ind].texture;
    }

    [[nodiscard]] SDL_Rect GetSrcRect() const {
        CC_ASSERT(!m_sprite_infos.empty(), "_INFO_:internal member: m_sprite_infos should not be empty!");
        return m_sprite_infos[m_ind].rect;
    }
    
   [[nodiscard]]  SpriteInfo& GetSpriteInfo() {
        CC_ASSERT(!m_sprite_infos.empty(), "_INFO_:internal member: m_sprite_infos should not be empty!");
        return m_sprite_infos[m_ind];
    } 

    [[nodiscard]] b2Fixture* GetFixture() const {
        return m_fixture;
    }
    
    void AddSpriteInfo(const SpriteInfo& info) {
        m_sprite_infos.push_back(info);
    }

    virtual void UpdateRenderCandidate() {
        m_ind = 0;
    }

protected:
    void SetState(std::size_t ind) {
        CC_ASSERT(ind < m_sprite_infos.size(), "_INFO_: index out of range!");
        m_ind = ind;
    }

private:
    // SDL_Renderer* m_renderer; // is this necessary?
    std::vector< SpriteInfo > m_sprite_infos;

    std::string m_sprite_name;

    Entity* entity;
    // entt::entity m_entity_handle;

    b2Fixture* m_fixture; // fixture and sprite is a one-to-one relation!

    std::size_t           m_ind;
    std::filesystem::path m_texture_path;
};