#include "Box.h"

#include <iostream>
#include <stdexcept>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_error.h>
#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Camera.h"
#include "Log.h"

// #include "Camera.h"

Box::Box(b2World* world, SDL_Renderer* renderer) : Entity(world, renderer) {
    CC_CORE_INFO("box entity created.");
}

Box::~Box() {
    auto pw = m_body->GetPosition();
    auto ps = g_camera.ConvertWorldToScreen(pw);
    CC_CORE_INFO("box destroyed, location at {}, {}", ps.x, ps.y);
    SDL_DestroyTexture(m_BoxTexture);
}

void Box::Init(b2Vec2 originalPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle) {
    LoadBoxToWorld(originalPos, boxSize, originalVel, originalAngle);
    LoadTexture();

    m_box_rect.w = g_camera.ConvertWorldToScreen(boxSize.x);
    m_box_rect.h = g_camera.ConvertWorldToScreen(boxSize.y);
    CC_CORE_INFO("box size info: box.w = {}, box.h = {}", m_box_rect.w, m_box_rect.h);
}

void Box::LoadBoxToWorld(b2Vec2 originPos, b2Vec2 boxSize, b2Vec2 originalVel, float originalAngle) {
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

void Box::LoadTexture() {
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

void Box::Render() { 
    auto pw = m_body->GetPosition();
    auto ps = g_camera.ConvertWorldToScreen(pw);
    m_box_rect.x = static_cast<int>(ps.x); 
    m_box_rect.y = static_cast<int>(ps.y);

    if(SDL_RenderCopyEx(m_renderer, m_BoxTexture, NULL, &m_box_rect, GetAngleDegree(), NULL, SDL_FLIP_NONE)) {
        CC_CORE_ERROR("SDL_RenderCopyEx failed to render entity box, Error message: {}", SDL_GetError());
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}

float Box::GetAngleDegree() {
    return m_body->GetAngle() * 180.0f / M_PI;
}