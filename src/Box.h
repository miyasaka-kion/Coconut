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
    virtual ~Box();
// >>>>>>>> API begin >>>>>>>>>
public:
    void init(b2Vec2 originalPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle);  // override?
    void render() override;

    int   getPosPixX();
    int   getPosPixY();
    float getAngleDegree();
// >>>>>>>> API end  >>>>>>>>>
private:
    void updateRect();
    void addToWorld(b2Vec2 originPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle);

private:
    SDL_Texture* m_BoxTexture;
    void         loadTexture();

public:
    SDL_Rect       m_BoxRect;
    b2Body*        m_body;
    b2PolygonShape m_polygonShape;
};