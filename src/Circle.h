#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>

#include "Entity.h"

class Circle : public Entity {
public:
    Circle() = delete;
    Circle(b2World* world, SDL_Renderer* renderer);
    virtual ~Circle();
    // >>>>>>>> API begin >>>>>>>>>
public:
    void init(b2Vec2 originalPos, float radius, b2Vec2 orginalVel, float originalAngle);
    void render() override;

    int   getPosPixX();
    int   getPosPixY();
    float getAngleDegree();
    // >>>>>>>> API end >>>>>>>>>
private:
    void updateRect();
    void addToWorld();
    void loadTexture();

private:
    SDL_Texture* m_CircleTexture;

public:
    float         m_radius;
    SDL_Rect      m_BoxRect;
    b2Body*       m_body;
    b2CircleShape m_CircleShape;
};