#pragma once

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

class Entity {
public:
    Entity() = default;

    virtual ~Entity() {}
    virtual void render() = 0;

    // if the Entity is inActive, this entity should be removed from game
    bool isActive = 1;
    // to determin whether the entity should be rendered
    bool toBeRendered = true;
    
protected:
    Entity(b2World* world, SDL_Renderer* renderer) : m_world(world), m_renderer(renderer) {}

// parent info:
protected:
    SDL_Renderer* m_renderer;
    b2World*      m_world;
};
