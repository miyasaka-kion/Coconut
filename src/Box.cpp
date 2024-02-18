#include "Box.h"

#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_error.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Log.h"
#include "MetricConverter.h"

Box::Box(b2World* world, SDL_Renderer* renderer) : Entity(world, renderer) {
    CC_CORE_INFO("box entity created.");
}

Box::~Box() {
    CC_CORE_INFO("box destroyed, location at {}, {}", GetPosPixX(), GetPosPixX());
    SDL_DestroyTexture(m_BoxTexture);
}

void Box::Init(b2Vec2 originalPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle) {
    loadBoxToWorld(originalPos, boxSize, originalVel, originalAngle);
    loadTexture();

    m_box_rect.w = MetricConverter::toPix(boxSize.x);
    m_box_rect.h = MetricConverter::toPix(boxSize.y);
    CC_CORE_INFO("box size info: box.w = {}, box.h = {}", m_box_rect.w, m_box_rect.h);
}

void Box::loadBoxToWorld(b2Vec2 originPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle) {
    CC_CORE_INFO("Calling loadBoxToWorld");
    b2BodyDef boxBodyDef;
    boxBodyDef.type     = b2_dynamicBody;
    boxBodyDef.angle    = originalAngle;  // flips the whole thing -> 180 grad drehung
    boxBodyDef.position = originPos;

    m_body = m_world->CreateBody(&boxBodyDef);
    m_body->SetLinearVelocity(originalVel);

    b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox((boxSize.x / 2.0f) - dynamicBox.m_radius, (boxSize.y / 2.0f) - dynamicBox.m_radius);

    CC_CORE_INFO("box info:  hx: {}, hy: {}", (boxSize.x / 2.0f) - dynamicBox.m_radius, (boxSize.y / 2.0f) - dynamicBox.m_radius);

    b2FixtureDef fixtureDef;
    fixtureDef.shape       = &dynamicBox;
    fixtureDef.density     = 1;
    fixtureDef.friction    = 0.1f;
    fixtureDef.restitution = 0.5f;
    m_body->CreateFixture(&fixtureDef);
}

void Box::loadTexture() {
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites) {
        CC_CORE_ERROR("loadTexture(): box.png failed to load! Error msg: {}", SDL_GetError());
        throw std::runtime_error("loadTexture():  tmp_sprites is NULL");
        // throw some error here and somehow return(TODO)
    }

    m_BoxTexture = SDL_CreateTextureFromSurface(m_renderer, tmp_sprites);
    if(m_BoxTexture == NULL) {
        CC_CORE_ERROR("Create texture from surface failed! Error msg: {}", SDL_GetError());
        throw std::runtime_error("texture_box is NULL");
    }
    CC_CORE_INFO("loadTexture(): Box texture loaded.");
    SDL_FreeSurface(tmp_sprites);
}

int Box::GetPosPixX() {
    return MetricConverter::toPixX(m_body->GetPosition().x) - m_box_rect.w / 2.0f;
}

int Box::GetPosPixY() {
    return MetricConverter::toPixY(m_body->GetPosition().y) - m_box_rect.h / 2.0f;
}

void Box::Render() { 
    std::tie(m_box_rect.x, m_box_rect.y) = std::make_tuple(this->GetPosPixX(), this->GetPosPixY());

    if(SDL_RenderCopyEx(m_renderer, m_BoxTexture, NULL, &m_box_rect, GetAngleDegree(), NULL, SDL_FLIP_NONE)) {
        CC_CORE_ERROR("SDL_RenderCopyEx failed to render entity box, Error message: {}", SDL_GetError());
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}

float Box::GetAngleDegree() {
    return MetricConverter::toDegree(m_body->GetAngle());
}