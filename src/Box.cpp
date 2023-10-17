#include "Box.h"

#include <iostream>
#include <stdexcept>


#include <box2d/box2d.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL_error.h>
#include <SDL_render.h>

#include "Constants.h"
#include "MetricConverter.h"
#include "Log.h"

Box::Box(b2World* world, SDL_Renderer* renderer): Entity(world, renderer) {
    loadBoxToWorld(b2Vec2(xOriginPosition, yOriginPosition) , vel, originalAngle); // not finished
    loadTexture();

    box_rect.w = MetricConverter::toPix(OriginWidth);
    box_rect.h = MetricConverter::toPix(OriginHeight);
    CC_CORE_INFO( "box size info: \n box.w = {}, box.h = {}", box_rect.w, box_rect.h);
}

Box::~Box() {
    spdlog::info("box destroyed, location at {}, {}", getPosPixX(), getPosPixX());
    SDL_DestroyTexture(boxTexture);
}

void Box::loadBoxToWorld(b2Vec2 originPos, b2Vec2 originVelocity, float originAngle) {    
    CC_CORE_INFO("Calling loadBoxToWorld");
    b2BodyDef boxBodyDef;
    boxBodyDef.type  = b2_dynamicBody;
    boxBodyDef.angle = originAngle;  // flips the whole thing -> 180 grad drehung
    boxBodyDef.position = originPos;

    body = m_world->CreateBody(&boxBodyDef);
    body->SetLinearVelocity(originVelocity);

    // b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(
        (OriginWidth/ 2.0f) - dynamicBox.m_radius, 
        (OriginHeight / 2.0f) - dynamicBox.m_radius
    );  

    CC_CORE_INFO("box info: \n hx: {}, \n hy: {}", (OriginWidth / 2.0f) - dynamicBox.m_radius, (OriginHeight / 2.0f) - dynamicBox.m_radius );

    b2FixtureDef fixtureDef;
    fixtureDef.shape       = &dynamicBox;
    fixtureDef.density     = 1;
    fixtureDef.friction    = 0.1f;
    fixtureDef.restitution = 0.5f; 
    body->CreateFixture(&fixtureDef);
}

void Box::loadTexture() {
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites) {
        CC_CORE_INFO("loadTexture(): box.png failed to load!");
        throw std::runtime_error("loadTexture():  tmp_sprites is NULL");
        // throw some error here and somehow return(TODO)
    }

    boxTexture = SDL_CreateTextureFromSurface(m_renderer, tmp_sprites);
    if(boxTexture == NULL) {
        std::cout << "texture_box initialization failed, Error message: " << SDL_GetError() << std::endl;
        throw std::runtime_error("texture_box is NULL");
    }
    CC_CORE_INFO("loadTexture(): Box texture loaded.");
    SDL_FreeSurface(tmp_sprites);
}

int Box::getPosPixX() {
    return MetricConverter::toPixX(body->GetPosition().x) - box_rect.w / 2.0f;
}

int Box::getPosPixY() {
    return MetricConverter::toPixY(body->GetPosition().y) - box_rect.h / 2.0f;
}

void Box::updateBoxPixelCoordinate() {
    box_rect.x = this->getPosPixX();
    box_rect.y = this->getPosPixY();
}

void Box::render() {
    updateBoxPixelCoordinate();
    if(SDL_RenderCopyEx(m_renderer, boxTexture, NULL, &box_rect, getAngleDegree(), NULL, SDL_FLIP_NONE)){
        CC_CORE_ERROR("SDL_RenderCopyEx failed to render entity box, Error message: {}", SDL_GetError());
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}

float Box::getAngleDegree() {
    return MetricConverter::toDegree(body->GetAngle());
}