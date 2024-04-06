#include "QuadWrite.h"

#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_render.h>

#include "Core/Camera.h"
#include "Core/Log.h"
#include "Render/SpriteLoader.h"
#include "Util/sdl_check.h"

/**
 * Render function to display sprite at a specified position with a given angle.
 *
 * @param info Pointer to the SpriteInfo struct containing texture and rectangle information
 * @param box_size Size of the box (world coordinates) enclosing the sprite
 * @param pw World position of the point where the sprite begin to be rendered, and the sprite start to render on the upper left corner
 * @param angle Angle at which the sprite should be rendered
 *
 * @return void
 *
 * @throws None
 */
void QuadWrite::Render(SpriteInfo* info, const b2Vec2& box_size, const b2Vec2& pw, const float angle) {
    SDL_Rect m_dst_rect;
    m_dst_rect.h = g_camera.ConvertWorldToScreen(box_size.y);
    m_dst_rect.w = g_camera.ConvertWorldToScreen(box_size.x);

    auto pw_offset         = pw + b2Vec2(-box_size.x / 2.0f, box_size.y / 2.0f);  // box offset

    auto ps      = g_camera.ConvertWorldToScreen(pw_offset);
    m_dst_rect.x = static_cast< int >(ps.x);
    m_dst_rect.y = static_cast< int >(ps.y);

    SDL_CHECK(SDL_RenderCopyEx(m_renderer, info->texture, &info->rect, &m_dst_rect, -angle * 180.0f / M_PI, NULL, SDL_FLIP_NONE));
}