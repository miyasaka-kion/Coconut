#pragma once

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

class QuadWrite {
public:
    QuadWrite() = delete;
    QuadWrite(SDL_Renderer* renderer) : m_renderer(renderer) {}
    virtual ~QuadWrite() = default;

    void UpdateRenderInfo(SDL_Texture* m_texture, const b2Vec2& box_size, const b2Vec2& pw, const float angle);
    void Render();

private:
    SDL_Renderer* m_renderer;

    // these vars should be UPDATED before calling Render()
    SDL_Texture* m_texture;
    b2Vec2       m_box_size;
    b2Vec2       m_pw;
    float        angle;  // in rad, i.e. the standard of box2d
};
