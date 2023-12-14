// #include "Car.h"

// #include <SDL_image.h>
// #include <SDL_render.h>

// #include "Log.h"
// #include "MetricConverter.h"

// Car::Car(b2World* world, SDL_Renderer* renderer) : Entity(world, renderer) {
//     CC_CORE_INFO("Car entity created!");
// }

// Car::~Car() {
//     CC_CORE_INFO("Car destroyed, location at {}, {}", getPosPixX(), getPosPixX());
//     SDL_DestroyTexture(m_CarTexture);
// }

// // >>>>>>>> API begin >>>>>>>>>
// void Car::init(b2Vec2 originalPos, float radius, b2Vec2 orginalVel, float originalAngle) {
//     m_radius = radius;
//     addToWorld();
//     loadTexture();

//     m_BoxRect.w = m_BoxRect.h = MetricConverter::toPix(radius * 2);
// }

// void Car::render() {
//     updateRect();

//     // CC_CORE_INFO("Car pos {}, {}", getPosPixX(), getPosPixY());
//     CC_CORE_INFO("m_BoxRect, size = {}, {}", m_BoxRect.w, m_BoxRect.h);
//     CC_CORE_INFO("m_BoxRect, pos = {}, {}", m_BoxRect.x, m_BoxRect.y);

//     if(SDL_RenderCopyEx(m_renderer, m_CarTexture, NULL, &m_BoxRect, getAngleDegree(), NULL, SDL_FLIP_NONE)) {
//         CC_CORE_ERROR("SDL_RenderCopyEx failed to render empty texture, {}", SDL_GetError());
//         throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
//     }
// }

// int Car::getPosPixX() {
//     return MetricConverter::toPixX(m_body->GetPosition().x) - m_BoxRect.w / 2.0f;
// }

// int Car::getPosPixY() {
//     return MetricConverter::toPixY(m_body->GetPosition().y) - m_BoxRect.h / 2.0f;
// }

// float Car::getAngleDegree() {
//     return MetricConverter::toDegree(m_body->GetAngle());
// }

// // >>>>>>>> API end >>>>>>>>>

// void Car::updateRect() {
//     m_BoxRect.x = this->getPosPixX();
//     m_BoxRect.y = this->getPosPixY();
// }

// void Car::addToWorld() {
//     CC_CORE_INFO("Calling loadCarToWorld");
//     b2BodyDef bd;
//     bd.type = b2_dynamicBody;
//     // bd.position.Set(0.0f, 0.0f);
//     // almost use the default body def.

//     m_body = m_world->CreateBody(&bd);

//     // m_CarShape.m_p.Set(0.0f, 0.0f);
//     m_CarShape.m_radius = m_radius;

//     CC_CORE_INFO("m_CarShape: x = {}, pos.y = {}", m_CarShape.m_p.x, m_CarShape.m_p.y);
//     CC_CORE_INFO("m_CarShape: radius = {}", m_CarShape.m_radius);

//     b2FixtureDef fd;
//     fd.shape   = &m_CarShape;
//     fd.density = 1;
//     // fd.friction    = 0.1f;
//     // fd.restitution = 0.5f;

//     m_body->CreateFixture(&fd);
// }

// void Car::loadTexture() {
//     IMG_Init(IMG_INIT_PNG);
//     SDL_Surface* tmp_sprites;
//     tmp_sprites = IMG_Load("assets/box.png");
//     if(!tmp_sprites) {
//         CC_CORE_ERROR("loadTexture(): box.png failed to load! Error msg: {}", SDL_GetError());
//         throw std::runtime_error("loadTexture():  tmp_sprites is NULL");
//         // throw some error here and somehow return(TODO)
//     }

//     m_CarTexture = SDL_CreateTextureFromSurface(m_renderer, tmp_sprites);
//     if(m_CarTexture == NULL) {
//         CC_CORE_ERROR("Create texture from surface failed! Error msg: {}", SDL_GetError());
//         throw std::runtime_error("texture_box is NULL");
//     }
//     CC_CORE_INFO("loadTexture(): Car texture loaded.");
//     SDL_FreeSurface(tmp_sprites);
// }
