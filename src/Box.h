#pragma once

#include <tuple>

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Entity.h"

// Box is and Entity, should be inherit an Entity class here...
class Box : public Entity {
public:
    Box() = delete;
    Box(b2World* world, SDL_Renderer* renderer);
    // Box(const Box&) = delete; // this is unsure
    ~Box();


    void Init(b2Vec2 originalPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle);  // override?
    void Render() override;

    float GetAngleDegree(); 

    void LoadBoxToWorld(b2Vec2 originPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle);

private:
    SDL_Texture* m_BoxTexture;
    void         LoadTexture();

public:
    SDL_Rect       m_box_rect;
    b2Body*        m_body;
    

private:
    // TODO: an Initial position of the box, when pressing r all entities should revert to its original position

private:
    b2Vec2 m_boxSize;
};