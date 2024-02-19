#include "Car.h"

#include <SDL_image.h>
#include <SDL_render.h>

#include "Camera.h"
#include "Log.h"

Car::Car(b2World* world, SDL_Renderer* renderer) : Entity(world, renderer) {
    CC_CORE_INFO("Car entity created!");
}

Car::~Car() {
    auto pw = m_car->GetPosition();
    auto ps = g_camera.ConvertWorldToScreen(pw);
    CC_CORE_INFO("Car destroyed, location at {}, {}", ps.x, ps.y);
    SDL_DestroyTexture(m_CarTexture);
}

// >>>>>>>> API begin >>>>>>>>>
void Car::Init(b2Vec2 originalPos, float radius, b2Vec2 orginalVel, float originalAngle) {
    m_radius = radius;
    AddToWorld();
    LoadTexture();

    m_BoxRect.w = m_BoxRect.h = g_camera.ConvertWorldToScreen(radius * 2);
    ;
}

void Car::Render() {
    UpdateRect();

    // CC_CORE_INFO("Car pos {}, {}", getPosPixX(), getPosPixY());
    CC_CORE_INFO("m_BoxRect, size = {}, {}", m_BoxRect.w, m_BoxRect.h);
    CC_CORE_INFO("m_BoxRect, pos = {}, {}", m_BoxRect.x, m_BoxRect.y);

    if(SDL_RenderCopyEx(m_renderer, m_CarTexture, NULL, &m_BoxRect, GetAngleDegree(), NULL, SDL_FLIP_NONE)) {
        CC_CORE_ERROR("SDL_RenderCopyEx failed to render empty texture, {}", SDL_GetError());
        throw std::runtime_error("SDL_RenderCopyEx failed to render entity box");
    }
}

float Car::GetAngleDegree() {
    // return MetricConverter::toDegree(m_car->GetAngle());
    return m_car->GetAngle() * 180.0f / M_PI;
}

// >>>>>>>> API end >>>>>>>>>

void Car::UpdateRect() {
    // m_BoxRect.x = this->GetPosPixX();
    // m_BoxRect.y = this->GetPosPixY();
    auto pw = m_car->GetPosition();
    auto ps = g_camera.ConvertWorldToScreen(pw);

    m_BoxRect.x = static_cast< int >(ps.x);
    m_BoxRect.y = static_cast< int >(ps.y);
}

void Car::AddToWorld() {
    CC_CORE_INFO("Calling loadCarToWorld");

    b2PolygonShape chassis;
    b2Vec2         vertices[8];
    vertices[0].Set(-1.5f, -0.5f);
    vertices[1].Set(1.5f, -0.5f);
    vertices[2].Set(1.5f, 0.0f);
    vertices[3].Set(0.0f, 0.9f);
    vertices[4].Set(-1.15f, 0.9f);
    vertices[5].Set(-1.5f, 0.2f);
    chassis.Set(vertices, 6);

    b2CircleShape circle;
    circle.m_radius = 0.4f;

    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(0.0f, 1.0f);
    m_car = m_world->CreateBody(&bd);
    m_car->CreateFixture(&chassis, 1.0f);

    b2FixtureDef fd;
    fd.shape    = &circle;
    fd.density  = 1.0f;
    fd.friction = 0.9f;

    bd.position.Set(-1.0f, 0.35f);
    m_wheel1 = m_world->CreateBody(&bd);
    m_wheel1->CreateFixture(&fd);

    bd.position.Set(1.0f, 0.4f);
    m_wheel2 = m_world->CreateBody(&bd);
    m_wheel2->CreateFixture(&fd);

    b2WheelJointDef jd;
    b2Vec2          axis(0.0f, 1.0f);

    float mass1 = m_wheel1->GetMass();
    float mass2 = m_wheel2->GetMass();

    float hertz        = 4.0f;
    float dampingRatio = 0.7f;
    float omega        = 2.0f * b2_pi * hertz;

    jd.Initialize(m_car, m_wheel1, m_wheel1->GetPosition(), axis);
    jd.motorSpeed       = 0.0f;
    jd.maxMotorTorque   = 20.0f;
    jd.enableMotor      = true;
    jd.stiffness        = mass1 * omega * omega;
    jd.damping          = 2.0f * mass1 * dampingRatio * omega;
    jd.lowerTranslation = -0.25f;
    jd.upperTranslation = 0.25f;
    jd.enableLimit      = true;
    m_spring1           = ( b2WheelJoint* )m_world->CreateJoint(&jd);

    jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
    jd.motorSpeed       = 0.0f;
    jd.maxMotorTorque   = 10.0f;
    jd.enableMotor      = false;
    jd.stiffness        = mass2 * omega * omega;
    jd.damping          = 2.0f * mass2 * dampingRatio * omega;
    jd.lowerTranslation = -0.25f;
    jd.upperTranslation = 0.25f;
    jd.enableLimit      = true;
    m_spring2           = ( b2WheelJoint* )m_world->CreateJoint(&jd);
}

void Car::LoadTexture() {
    IMG_Init(IMG_INIT_PNG);
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites) {
        CC_CORE_ERROR("loadTexture(): box.png failed to load! Error msg: {}", SDL_GetError());
        throw std::runtime_error("loadTexture():  tmp_sprites is NULL");
        // throw some error here and somehow return(TODO)
    }

    m_CarTexture = SDL_CreateTextureFromSurface(m_renderer, tmp_sprites);
    if(m_CarTexture == NULL) {
        CC_CORE_ERROR("Create texture from surface failed! Error msg: {}", SDL_GetError());
        throw std::runtime_error("texture_box is NULL");
    }
    CC_CORE_INFO("loadTexture(): Car texture loaded.");
    SDL_FreeSurface(tmp_sprites);
}
