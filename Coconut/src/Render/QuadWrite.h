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
    QuadWrite(SDL_Renderer* renderer) : m_renderer(renderer), info(nullptr) {}
    virtual ~QuadWrite() = default;

    void UpdateRenderInfo(SpriteInfo* info, const b2Vec2& box_size, const b2Vec2& pw, const float angle);
    void Render();

private:
    SDL_Renderer* m_renderer;

    SpriteInfo* info;
    b2Vec2      m_box_size;
    b2Vec2      m_pw;
    float       angle;  // in rad, i.e. the standard of box2d
};
