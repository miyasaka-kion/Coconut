#pragma once

#include <SDL_rect.h>
#include <SDL_render.h>

#include "Entity.h"

class Car : public Entity {
public:
    Car() = delete;
    Car(b2World* world, SDL_Renderer* renderer);
    virtual ~Car();

public:
    void Init(b2Vec2 originalPos, float radius, b2Vec2 orginalVel, float originalAngle);
    void Render() override;

    // int   GetPosPixX();
    // int   GetPosPixY();
    float GetAngleDegree();

private:
    void UpdateRect();
    void AddToWorld();
    void LoadTexture();

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