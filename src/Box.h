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
    void init(b2Vec2 originalPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle);  // override?
    void render() override;

    int   getPosPixX();
    int   getPosPixY();
    float getAngleDegree();

private:
    SDL_Texture* boxTexture;
    void         loadTexture();

    /// @param originPos original position of the box
    /// @param originVelocity original velocity of the box
    /// @param originAngle original angle of the box
    void loadBoxToWorld(b2Vec2 originPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle);

public:
    SDL_Rect       box_rect;
    b2Body*        body;
    b2PolygonShape dynamicBox;

private:
    // TODO: an Initial position of the box, when pressing r all entities should revert to its original position
};