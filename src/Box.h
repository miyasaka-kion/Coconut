#pragma once

#include <tuple>

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

    void updateBoxPixelCoordinate();
    void render();

    b2Vec2 getPosMeter();
    float getPosMeterX();
    float getPosMeterY();
    float getAngleRad();

    int getPosPixX();
    int getPosPixY();
    std::tuple<int, int> getPosPix();
    float getAngleDegree();
    
private:
    SDL_Texture* texture_box;
    void loadTexture();
    void loadBoxToWorld();

public:
    SDL_Rect box;
    b2Body*  body;

// parent info
private: 
    SDL_Renderer* m_renderer;
    b2World* m_world;
};