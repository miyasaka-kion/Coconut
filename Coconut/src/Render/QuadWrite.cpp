#include "QuadWrite.h"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "Core/Camera.h"
#include "Core/Log.h"
#include "Render/SpriteLoader.h"
#include "Util/sdl_check.h"

void QuadWrite::UpdateRenderInfo(SpriteInfo* info, const b2Vec2& box_size, const b2Vec2& pw, const float angle) {
    this->info = info;
    this->m_box_size = box_size;
    this->m_pw       = pw;
    this->angle      = angle;
}

void QuadWrite::Render() {
    SDL_Rect m_dst_rect;
    m_dst_rect.h = g_camera.ConvertWorldToScreen(m_box_size.x);
    m_dst_rect.w = g_camera.ConvertWorldToScreen(m_box_size.y);

    m_pw         = m_pw + b2Vec2(-m_box_size.x / 2.0f, m_box_size.y / 2.0f);
    auto ps      = g_camera.ConvertWorldToScreen(m_pw);
    m_dst_rect.x = static_cast< int >(ps.x);
    m_dst_rect.y = static_cast< int >(ps.y);

    SDL_CHECK(SDL_RenderCopyEx(m_renderer, info->texture, &info->rect, &m_dst_rect, -angle * 180.0f / M_PI, NULL, SDL_FLIP_NONE));
}