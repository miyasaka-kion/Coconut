#pragma once

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <box2d/box2d.h>


// Box is and Entity, should be inherit an Entity class here...
class Box {
public:
    Box() = default;
    Box(b2World* world, SDL_Renderer* renderer);
    // Box(const Box&) = delete; // this is unsure
    ~Box();


    void render();

private:
    SDL_Texture* texture_box;
    void loadTexture();
    void loadBoxToWorld();

public:
    SDL_Rect box;
    b2Body*  Body;

// parent info
private: 
    SDL_Renderer* m_renderer;
    b2World* m_world;
};