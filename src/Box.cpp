#include "Box.h"

#include <SDL_render.h>
#include <iostream>

#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdexcept>

#include "Constants.h"
#include "MetricConverter.h"

Box::Box(b2World* world, SDL_Renderer* renderer): m_world(world), m_renderer(renderer) {
    loadBoxToWorld();
    loadTexture();

    box.w = MetricConverter::toPix(w_box);
    box.h = MetricConverter::toPix(h_box);
}

Box::~Box() {
    SDL_DestroyTexture(texture_box);
}

void Box::loadBoxToWorld() {
    b2BodyDef boxBodyDef;
    boxBodyDef.type  = b2_dynamicBody;
    boxBodyDef.angle = angle_box;  // flips the whole thing -> 180 grad drehung
    // boxBodyDef.angle = 0;
    boxBodyDef.position.Set(x_box, y_box);

    // velocity vel created here (x)
    // velocity vel created in Constants.h
    // b2Vec2 vel{ 1.0f, 0.2f };

    body = m_world->CreateBody(&boxBodyDef);
    body->SetLinearVelocity(vel);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(
        (w_box / 2.0f) - dynamicBox.m_radius, 
        (h_box / 2.0f) - dynamicBox.m_radius);  
        // will be 0.5 x 0.5

    // dynamicBox.SetAsBox((w_box / 2.0f) - 0.0f, (h_box / 2.0f) - 0.0f);  // will be 0.5 x 0.5
    std::cout << "dynamicBox.SetAsBox" << (w_box / 2.0f) - dynamicBox.m_radius << ' ' << (h_box / 2.0f) - dynamicBox.m_radius << std::endl;

    b2FixtureDef fixtureDef;
    fixtureDef.shape       = &dynamicBox;
    fixtureDef.density     = 1;
    fixtureDef.friction    = 0.3f;
    fixtureDef.restitution = 1.0f;  // modified
    body->CreateFixture(&fixtureDef);
}

void Box::loadTexture() {
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites) {
        std::cout << "box.png failed to load!" << std::endl;
        
        throw std::runtime_error("tmp_sprites is NULL");
        // throw some error here and somehow return(TODO)
    }

    texture_box = SDL_CreateTextureFromSurface(m_renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
}

void Box::updateBoxPixelCoordinate() {
    box.x = getPosPixX();
    box.y = getPosPixY();
}

void Box::render() {
    updateBoxPixelCoordinate();
    SDL_RenderCopyEx(m_renderer, texture_box, NULL, &box, getAngleDegree(), NULL, SDL_FLIP_NONE);
}

int Box::getPosPixX() {
    return MetricConverter::toPix(body->GetPosition().x);
}

int Box::getPosPixY() {
    return MetricConverter::toPix(body->GetPosition().y);
}

float Box::getAngleDegree() {
    return MetricConverter::toDegree(body->GetAngle());
}