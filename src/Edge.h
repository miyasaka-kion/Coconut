#pragma once

#include <SDL_render.h>
#include <box2d/box2d.h>
#include <SDL2/SDL.h>

#include "Entity.h"

// this should be inherited from a parent like Entity or Shape... (TODO)
class Edge : public Entity {
public:
    Edge() = default;
    Edge(b2World* world, SDL_Renderer* renderer);
    
    ~Edge();

public:
    b2EdgeShape edgeShape;
    b2Body* body;
    
    void render() override;

    int getPosPixX();
    int getPosPixY();

// parent info
private:
    SDL_Renderer* m_renderer;
    b2World* m_world;
};