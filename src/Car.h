#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>

#include "Entity.h"

class Car : public Entity {
public:
    Car() = delete;
    Car(b2World* world, SDL_Renderer* renderer);
    virtual ~Car();
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

    // SDL info
private:
    SDL_Texture* m_CarTexture;
    SDL_Rect     m_BoxRect;

    // physical features of a car
public:
    float m_radius;

    // b2 info
private:
    b2Body* m_car;
    b2Body* m_wheel1;
    b2Body* m_wheel2;

    float         m_speed;
    b2WheelJoint* m_spring1;
    b2WheelJoint* m_spring2;
};