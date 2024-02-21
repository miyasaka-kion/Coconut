#pragma once

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Entity.h"

class Edge : public Entity {
public:
    Edge() = default;
    Edge(b2World* world, SDL_Renderer* renderer);
    ~Edge();

    void Init(b2Vec2 startpoint, b2Vec2 endpoint) ; //override;

public:
    b2EdgeShape edgeShape;
    b2Body* body;
    
    void Render() override;
};