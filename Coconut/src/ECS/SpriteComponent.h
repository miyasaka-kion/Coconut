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

#include "util/sdl_delete.h"
#include "ECS/Entity.h"

struct SpriteComponent {
    SpriteComponent() = delete;
    SpriteComponent(SDL_Renderer* renderer, std::string_view texture_name);

    void                       LoadTexture();
    [[nodiscard]] SDL_Texture* GetTexture() const {
        return m_texture.get();
    }
    SDL_Renderer* m_renderer;
    SDL::Texture m_texture;

    std::string_view      m_texture_name;
    std::filesystem::path m_texture_path;
    
    Entity* entity;
    entt::entity m_entity_handle;
};