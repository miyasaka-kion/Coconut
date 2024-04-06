#pragma once

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <box2d/box2d.h>
#include <cstddef>

#include "Render/SpriteLoader.h"

// Draw Rectangle Sprites
class QuadWrite {
public:
    QuadWrite() = delete;
    QuadWrite(SDL_Renderer* renderer) : m_renderer(renderer) {}
    virtual ~QuadWrite() = default;

    void Render(SpriteInfo* info, const b2Vec2& box_size, const b2Vec2& pw, const float angle);

private:
    SDL_Renderer* m_renderer;
};
