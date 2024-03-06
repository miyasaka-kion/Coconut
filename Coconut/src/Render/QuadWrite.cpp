#include "QuadWrite.h"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "Core/Log.h"
#include "Core/Camera.h"


/**
 * Updates the render information of the QuadWrite object before it is rendered.
 *
 * @param m_texture The SDL texture to be updated.
 * @param box_size The size of the box.
 * @param pw The position and width.
 * @param angle The angle of rotation.
 *
 * @throws ErrorType Description of any potential errors.
 */
void QuadWrite::UpdateRenderInfo(SDL_Texture* m_texture, const b2Vec2& box_size, const b2Vec2& pw, const float angle) {
    this->m_texture = m_texture;
    this->m_box_size = box_size;
    this->m_pw = pw;
    this->angle = angle;
}

/**
 * Render function for QuadWrite class to render the entity box on the screen.
 *
 * @param None
 *
 * @return None
 *
 * @throws std::runtime_error if SDL_RenderCopyEx fails to render entity box
 */
void QuadWrite::Render() {

    SDL_Rect m_dst_rect;
    m_dst_rect.h = g_camera.ConvertWorldToScreen(m_box_size.x);
    m_dst_rect.w = g_camera.ConvertWorldToScreen(m_box_size.y);

    m_pw         = m_pw + b2Vec2(-m_box_size.x / 2.0f, m_box_size.y / 2.0f);
    auto ps      = g_camera.ConvertWorldToScreen(m_pw);
    m_dst_rect.x = static_cast< int >(ps.x);
    m_dst_rect.y = static_cast< int >(ps.y);

    if(SDL_RenderCopyEx(m_renderer, m_texture, NULL, &m_dst_rect, -angle * 180.0f / M_PI, NULL, SDL_FLIP_NONE)) {
        CC_CORE_ERROR("SDL_RenderCopyEx failed to render entity box, Error message: {}", SDL_GetError());
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}