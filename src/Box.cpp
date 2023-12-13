#include "Box.h"

#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_error.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

// #include "Constants.h"
#include "Log.h"
#include "MetricConverter.h"

Box::Box(b2World* world, SDL_Renderer* renderer) : Entity(world, renderer) {
    CC_CORE_INFO("box entity created.");
}

Box::~Box() {
    CC_CORE_INFO("box destroyed, location at {}, {}", getPosPixX(), getPosPixX());
    SDL_DestroyTexture(m_BoxTexture);
}

void Box::init(b2Vec2 originalPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle) {
    addToWorld(originalPos, boxSize, originalVel, originalAngle);  // not finished
    loadTexture();

    m_BoxRect.w = MetricConverter::toPix(boxSize.x);
    m_BoxRect.h = MetricConverter::toPix(boxSize.y);
    CC_CORE_INFO("box size info: box.w = {}, box.h = {}", m_BoxRect.w, m_BoxRect.h);
}

void Box::addToWorld(b2Vec2 originPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle) {
    CC_CORE_INFO("Calling loadBoxToWorld");
    b2BodyDef bd;
    bd.type     = b2_dynamicBody;
    bd.angle    = originalAngle;  // flips the whole thing -> 180 grad drehung
    bd.position = originPos;
    bd.position.Set(1.0f, 1.0f);

    m_body = m_world->CreateBody(&bd);
    m_body->SetLinearVelocity(originalVel);

    // b2PolygonShape dynamicBox;
    m_polygonShape.SetAsBox((boxSize.x / 2.0f) - m_polygonShape.m_radius, (boxSize.y / 2.0f) - m_polygonShape.m_radius);

    {
        CC_CORE_INFO("box info:  hx: {}, hy: {}", (boxSize.x / 2.0f) - m_polygonShape.m_radius, (boxSize.y / 2.0f) - m_polygonShape.m_radius);
        CC_CORE_INFO("box radius = {}", m_polygonShape.m_radius);
    }

    b2FixtureDef fd;
    fd.shape       = &m_polygonShape;
    fd.density     = 1;
    fd.friction    = 0.1f;
    fd.restitution = 0.5f;
    m_body->CreateFixture(&fd);
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

int Box::getPosPixX() {
    return MetricConverter::toPixX(m_body->GetPosition().x) - m_BoxRect.w / 2.0f;
}

int Box::getPosPixY() {
    return MetricConverter::toPixY(m_body->GetPosition().y) - m_BoxRect.h / 2.0f;
}

void Box::updateRect() {
    m_BoxRect.x = this->getPosPixX();
    m_BoxRect.y = this->getPosPixY();
}

void Box::render() {
    updateRect();
    // body->ApplyTorque(0.05f, true);
    // body->ApplyAngularImpulse(0.1f, true);

    // CC_CORE_INFO("box pos {}, {}", getPosPixX(), getPosPixY());
    
    if(SDL_RenderCopyEx(m_renderer, m_BoxTexture, NULL, &m_BoxRect, getAngleDegree(), NULL, SDL_FLIP_NONE)) {
        CC_CORE_ERROR("SDL_RenderCopyEx failed to render entity box, Error message: {}", SDL_GetError());
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}

float Box::getAngleDegree() {
    return MetricConverter::toDegree(m_body->GetAngle());
}