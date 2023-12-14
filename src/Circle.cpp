#include "Circle.h"

#include <SDL_image.h>
#include <SDL_render.h>

#include "Log.h"
#include "MetricConverter.h"

Circle::Circle(b2World* world, SDL_Renderer* renderer) : Entity(world, renderer) {
    CC_CORE_INFO("Circle entity created!");
}

Circle::~Circle() {
    CC_CORE_INFO("circle destroyed, location at {}, {}", getPosPixX(), getPosPixX());
    SDL_DestroyTexture(m_CircleTexture);
}

// >>>>>>>> API begin >>>>>>>>>
void Circle::init(b2Vec2 originalPos, float radius, b2Vec2 orginalVel, float originalAngle) {
    m_radius = radius;
    addToWorld();
    loadTexture();

    m_BoxRect.w = m_BoxRect.h = MetricConverter::toPix(radius * 2);
}

void Circle::render() {
    updateRect();

    // CC_CORE_INFO("circle pos {}, {}", getPosPixX(), getPosPixY());
    CC_CORE_INFO("m_BoxRect, size = {}, {}", m_BoxRect.w, m_BoxRect.h);
    CC_CORE_INFO("m_BoxRect, pos = {}, {}", m_BoxRect.x, m_BoxRect.y);

    if(SDL_RenderCopyEx(m_renderer, m_CircleTexture, NULL, &m_BoxRect, getAngleDegree(), NULL, SDL_FLIP_NONE)) {
        CC_CORE_ERROR("SDL_RenderCopyEx failed to render empty texture, {}", SDL_GetError());
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}

int Circle::getPosPixX() {
    return MetricConverter::toPixX(m_body->GetPosition().x) - m_BoxRect.w / 2.0f;
}

int Circle::getPosPixY() {
    return MetricConverter::toPixY(m_body->GetPosition().y) - m_BoxRect.h / 2.0f;
}

float Circle::getAngleDegree() {
    return MetricConverter::toDegree(m_body->GetAngle());
}

// >>>>>>>> API end >>>>>>>>>

void Circle::updateRect() {
    m_BoxRect.x = this->getPosPixX();
    m_BoxRect.y = this->getPosPixY();
}

void Circle::addToWorld() {
    CC_CORE_INFO("Calling loadCircleToWorld");
    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    // bd.position.Set(0.0f, 0.0f);
    // almost use the default body def.

    m_body = m_world->CreateBody(&bd);

    // m_CircleShape.m_p.Set(0.0f, 0.0f);
    m_CircleShape.m_radius = m_radius;

    CC_CORE_INFO("m_CircleShape: x = {}, pos.y = {}", m_CircleShape.m_p.x, m_CircleShape.m_p.y);
    CC_CORE_INFO("m_CircleShape: radius = {}", m_CircleShape.m_radius);

    b2FixtureDef fd;
    fd.shape = &m_CircleShape;
    // fd.density     = 1;
    // fd.friction    = 0.1f;
    // fd.restitution = 0.5f;

    m_body->CreateFixture(&fd);
}

void Circle::loadTexture() {
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites) {
        CC_CORE_ERROR("loadTexture(): box.png failed to load! Error msg: {}", SDL_GetError());
        throw std::runtime_error("loadTexture():  tmp_sprites is NULL");
        // throw some error here and somehow return(TODO)
    }

    m_CircleTexture = SDL_CreateTextureFromSurface(m_renderer, tmp_sprites);
    if(m_CircleTexture == NULL) {
        CC_CORE_ERROR("Create texture from surface failed! Error msg: {}", SDL_GetError());
        throw std::runtime_error("texture_box is NULL");
    }
    CC_CORE_INFO("loadTexture(): Circle texture loaded.");
    SDL_FreeSurface(tmp_sprites);
}
