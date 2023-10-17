#pragma once

#include <tuple>

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Entity.h"

// Box is and Entity, should be inherit an Entity class here...
class Box : public Entity {
public:
    Box() = default;
    Box(b2World* world, SDL_Renderer* renderer);
    // Box(const Box&) = delete; // this is unsure
    ~Box();

    void updateBoxPixelCoordinate();
    void render() override;

    int getPosPixX();
    int getPosPixY();
    float getAngleDegree();
    
private:
    SDL_Texture* boxTexture;
    void loadTexture();
    void loadBoxToWorld();

public:
    SDL_Rect box_rect;
    b2Body*  body;
    b2PolygonShape dynamicBox;

// parent info
private: 
    SDL_Renderer* m_renderer;
    b2World* m_world;
};