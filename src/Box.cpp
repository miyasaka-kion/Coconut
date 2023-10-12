#include "Box.h"

#include <SDL_error.h>
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

    box_rect.w = MetricConverter::toPix(w_box);
    std::cout << "box.w = "  <<  box_rect.w << std::endl;
    box_rect.h = MetricConverter::toPix(h_box);
    std::cout << "box.h = "  <<  box_rect.h << std::endl;
}

Box::~Box() {
    std::cout << "box destroyed" << std::endl;
    SDL_DestroyTexture(boxTexture);
}

void Box::loadBoxToWorld() {    
    std::cout << "loadBoxToWorld called." << std::endl;
    b2BodyDef boxBodyDef;
    boxBodyDef.type  = b2_dynamicBody;
    boxBodyDef.angle = angle_box;  // flips the whole thing -> 180 grad drehung
    boxBodyDef.position.Set(x_box, y_box);

    body = m_world->CreateBody(&boxBodyDef);
    body->SetLinearVelocity(vel);

    // b2PolygonShape dynamicBox;
    dynamicBox.SetAsBox(
        (w_box / 2.0f) - dynamicBox.m_radius, 
        (h_box / 2.0f) - dynamicBox.m_radius);  
        // will be 0.5 x 0.5
    
    std::cout << "box info: \n hx: " << (w_box / 2.0f) - dynamicBox.m_radius
    << "\n hy: " <<(h_box / 2.0f) - dynamicBox.m_radius << std::endl; 

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
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites) {
        std::cout << "loadTexture(): box.png failed to load!" << std::endl;
        throw std::runtime_error("loadTexture():  tmp_sprites is NULL");
        // throw some error here and somehow return(TODO)
    }

    boxTexture = SDL_CreateTextureFromSurface(m_renderer, tmp_sprites);
    if(boxTexture == NULL) {
        std::cout << "texture_box initialization failed, Error message: " << SDL_GetError() << std::endl;
        throw std::runtime_error("texture_box is NULL");
    }
    std::cout << "loadTexture(): Box texture loaded." << std::endl;
    SDL_FreeSurface(tmp_sprites);
}

int Box::getPosPixX() {
    return MetricConverter::toPixX(body->GetPosition().x);
}

int Box::getPosPixY() {
    return MetricConverter::toPixY(body->GetPosition().y);
}

void Box::updateBoxPixelCoordinate() {
    box_rect.x = this->getPosPixX();
    box_rect.y = this->getPosPixY();
}

void Box::render() {
    updateBoxPixelCoordinate();
    // std::cout << "Box position: " << box_rect.x << " " << box_rect.y << std::endl;

    // Returns 0 on success or a negative error code on failure; call SDL_GetError() for more
    if(SDL_RenderCopyEx(m_renderer, boxTexture, NULL, &box_rect, getAngleDegree(), NULL, SDL_FLIP_NONE)){
        std::cout << "SDL_RenderCopyEx failed to render entity box, Error message:"  << SDL_GetError() << std::endl;
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}

float Box::getAngleDegree() {
    return MetricConverter::toDegree(body->GetAngle());
}