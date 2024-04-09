#pragma once

#include <SDL2/SDL_render.h>
#include <string>

#include <box2d/box2d.h>

#include "Render/SpriteLoader.h"

class EdgeTile {
public:
    EdgeTile() = delete;

    EdgeTile(SDL_Renderer* renderer,  SpriteInfo& sprite_info, b2Vec2 start, b2Vec2 end, float width);

    void Render();

private:
    SDL_Renderer* m_renderer;
    b2Vec2        m_start;
    b2Vec2        m_end;

    b2Vec2      m_size;
    SpriteInfo  m_sprite_info;
};


