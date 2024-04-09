#include "QuadWrite.h"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "Core/Camera.h"
#include "Core/Log.h"
#include "Render/SpriteLoader.h"
#include "Util/sdl_check.h"


/**
 * Render function for QuadWrite class.
 *
 * @param info Pointer to SpriteInfo object
 * @param box_size Size of the box
 * @param pw Position vector, corresponding to the b2Body::GetPosition()
 * @param offset Indicates how the pw should be offset so that we draw from the upper left corner
 * @param angle Rotation angle
 *
 * @return void
 *
 * @throws ErrorType Description of potential errors
 */
void QuadWrite::Render(SpriteInfo* info, const b2Vec2& box_size, const b2Vec2& pw, const b2Vec2& offset, const float angle) {
    SDL_Rect m_dst_rect;
    m_dst_rect.h = g_camera.ConvertWorldToScreen(box_size.y);
    m_dst_rect.w = g_camera.ConvertWorldToScreen(box_size.x);

    // auto pw_offset         = pw + b2Vec2(-box_size.x / 2.0f, box_size.y / 2.0f);  // box offset
    auto pw_offset = pw + offset;

    auto ps      = g_camera.ConvertWorldToScreen(pw_offset);
    m_dst_rect.x = static_cast< int >(ps.x);
    m_dst_rect.y = static_cast< int >(ps.y);

    SDL_Point center = { static_cast< int >(-g_camera.ConvertWorldToScreen(offset.x)), static_cast< int >(g_camera.ConvertWorldToScreen(offset.y)) };

    SDL_CHECK(SDL_RenderCopyEx(m_renderer, info->texture, &info->rect, &m_dst_rect, -angle * 180.0f / M_PI, &center, SDL_FLIP_NONE));
}


/**
 * Render a sprite without bias.
 *
 * @param info Pointer to the sprite information
 * @param box_size Size of the box
 * @param pw Position vector, corresponding to the b2Body::GetPosition()
 * @param angle Angle of rotation
 *
 * @return void
 *
 * @throws None
 */
void QuadWrite::RenderNoBias(SpriteInfo* info, const b2Vec2& box_size, const b2Vec2& pw, const float angle) {
    Render(info, box_size, pw, b2Vec2(-box_size.x / 2.0f, box_size.y / 2.0f), angle);
}