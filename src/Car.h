#pragma once

#include <tuple>

#include <SDL2/SDL.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Entity.h"

class Car : public Entity {
public:
    Car() = default;
    Car(b2World* world, SDL_Renderer* renderer);

    ~Car();

    void updateBoxPixelCoordinate();
    void init(b2Vec2 originalPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle);  // override?
    void Render() override;

    int   getPosPixX();
    int   getPosPixY();
    float getAngleDegree();

private:
    SDL_Texture* m_Texture;
    void         loadTexture();

    void loadToWorld(b2Vec2 originPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle);


public: 
	b2Body* m_car;
	b2Body* m_wheel1;
	b2Body* m_wheel2;

	float m_speed;
	b2WheelJoint* m_spring1;
	b2WheelJoint* m_spring2;
};