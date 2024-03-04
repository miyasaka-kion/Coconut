#include "ECS/Sprite.h"

#include <string_view>

#include <SDL2/SDL_image.h>
#include <SDL_render.h>

#include "Camera.h"
#include "Body.h"

Sprite::Sprite(SDL_Renderer* renderer) : m_renderer(renderer) {
    m_boxSize = b2Vec2(2.0f, 2.0f);
}


Sprite::Sprite(SDL_Renderer* renderer, std::string_view texturePath) {
    m_renderer = renderer;
    m_path     = texturePath;

    if(!m_entity->HasComponent< Body >()) {
        std::runtime_error("Forgot to add Body Component!");
    }
}

void Sprite::LoadTexture() {
    CC_CORE_INFO("LoadTexture(): Loading texture...");
    IMG_Init(IMG_INIT_PNG);

    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites) {
        CC_CORE_ERROR("LoadTexture(): box.png failed to load! Error msg: {}", SDL_GetError());
        throw std::runtime_error("LoadTexture():  tmp_sprites is NULL");
    }

    m_texture = SDL_CreateTextureFromSurface(m_renderer, tmp_sprites);
    if(m_texture == NULL) {
        CC_CORE_ERROR("Create texture from surface failed! Error msg: {}", SDL_GetError());
        throw std::runtime_error("texture_box is NULL");
    }
    CC_CORE_INFO("LoadTexture(): Box texture loaded.");
    SDL_FreeSurface(tmp_sprites);
}

void Sprite::Init() {
    if(m_entity->HasComponent<Body>() == false) {
        std::runtime_error("Forgot to add Body Component!");
    }
    m_body = m_entity->GetComponent<Body>().GetBody();
    LoadTexture();
}

void Sprite::Render() {
    m_box_rect.h = g_camera.ConvertWorldToScreen(m_boxSize.x);
    m_box_rect.w = g_camera.ConvertWorldToScreen(m_boxSize.y);

    auto pw      = m_body->GetPosition();
    pw           = pw + b2Vec2(- m_boxSize.x / 2.0f, m_boxSize.y / 2.0f);
    auto ps      = g_camera.ConvertWorldToScreen(pw);
    m_box_rect.x = static_cast< int >(ps.x);
    m_box_rect.y = static_cast< int >(ps.y);

    if(SDL_RenderCopyEx(m_renderer, m_texture, NULL, &m_box_rect, -m_body->GetAngle() * 180.0f / M_PI, NULL, SDL_FLIP_NONE)) {
        CC_CORE_ERROR("SDL_RenderCopyEx failed to render entity box, Error message: {}", SDL_GetError());
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}