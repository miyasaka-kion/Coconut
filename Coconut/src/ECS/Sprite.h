#pragma once

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
#include <SDL_render.h>
#include <stdexcept>

#include "ECS/Entity.h"


class Sprite : public Component {
public:
    Sprite() = delete;
    Sprite(SDL_Renderer* renderer); // debug
    Sprite(SDL_Renderer* renderer, std::string_view texturePath);

    void Init() override;
    void Render() override;

    void LoadTexture();

private:
    SDL_Renderer* m_renderer;
    SDL_Texture* m_texture;
    SDL_Rect m_box_rect; // render box

private: 
    b2Body* m_body;
    b2Vec2 m_boxSize;
private:
    std::string_view m_path;
};