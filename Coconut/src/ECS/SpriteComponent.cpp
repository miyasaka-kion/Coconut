#include "ECS/SpriteComponent.h"

#include <string_view>
#include <string>
#include <filesystem>

#include <SDL2/SDL_image.h>
#include <SDL_render.h>

#include "Core/Log.h"
#include "util/sdl_delete.h"

#ifndef COCONUT_ASSET_PATH
    // CC_CORE_ERROR("COCONUT_ASSET_PATH is not defined!");
    throw std::runtime_error("COCONUT_ASSET_PATH is not defined!");
#endif

SpriteComponent::SpriteComponent(SDL_Renderer* renderer, std::string_view texture_name) {
    m_renderer = renderer;
    m_texture_name = texture_name;
    m_texture_path = std::filesystem::path(COCONUT_ASSET_PATH) /texture_name;   
    LoadTexture();
}

void SpriteComponent::LoadTexture() {
    CC_CORE_INFO("LoadTexture(): Loading texture, texture path: {}", m_texture_path.string());
    CC_CORE_INFO("LoadTexture(): Loading texture, texture name: {}", m_texture_name);
    IMG_Init(IMG_INIT_PNG);

    SDL::Surface tmp_sprites {IMG_Load(m_texture_path.string().c_str())};

    if(!tmp_sprites) {
        CC_CORE_ERROR("LoadTexture(): box.png failed to load! Error msg: {}", SDL_GetError());
        throw std::runtime_error("LoadTexture():  tmp_sprites is NULL");
    }

    m_texture = SDL::Texture(SDL_CreateTextureFromSurface(m_renderer, tmp_sprites.get()));
    if(m_texture == NULL) {
        CC_CORE_ERROR("Create texture from surface failed! Error msg: {}", SDL_GetError());
        throw std::runtime_error("texture_box is NULL");
    }

    CC_CORE_INFO("LoadTexture(): Box texture loaded.");
}

