#include "Car.h"

#include <SDL2/SDL_keycode.h>
#include <memory>

#include "ECS/Components.h"
#include "Render/SpriteLoader.h"
#include "UI/Layer.h"
#include "imgui.h"

#include "Coconut.h"

Car::Car(GameContext* gc) : m_gc(gc) {
    auto world = gc->GetWorld();
    m_speed    = 50.0f;

    b2Body* ground = NULL;
    {
        b2BodyDef bd;
        ground = world->CreateBody(&bd);

        b2EdgeShape shape;

        b2FixtureDef fd;
        fd.shape    = &shape;
        fd.density  = 0.0f;
        fd.friction = 0.6f;

        shape.SetTwoSided(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
        ground->CreateFixture(&fd);

        float hs[10] = { 0.25f, 1.0f, 4.0f, 0.0f, 0.0f, -1.0f, -2.0f, -2.0f, -1.25f, 0.0f };

        float x = 20.0f, y1 = 0.0f, dx = 5.0f;

        for(int32 i = 0; i < 10; ++i) {
            float y2 = hs[i];
            shape.SetTwoSided(b2Vec2(x, y1), b2Vec2(x + dx, y2));
            ground->CreateFixture(&fd);
            y1 = y2;
            x += dx;
        }

        for(int32 i = 0; i < 10; ++i) {
            float y2 = hs[i];
            shape.SetTwoSided(b2Vec2(x, y1), b2Vec2(x + dx, y2));
            ground->CreateFixture(&fd);
            y1 = y2;
            x += dx;
        }

        shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
        ground->CreateFixture(&fd);

        x += 80.0f;
        shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
        ground->CreateFixture(&fd);

        x += 40.0f;
        shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 10.0f, 5.0f));
        ground->CreateFixture(&fd);

        x += 20.0f;
        shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x + 40.0f, 0.0f));
        ground->CreateFixture(&fd);

        x += 40.0f;
        shape.SetTwoSided(b2Vec2(x, 0.0f), b2Vec2(x, 20.0f));
        ground->CreateFixture(&fd);
    }

    // Teeter
    {
        b2BodyDef bd;
        bd.position.Set(140.0f, 1.0f);
        bd.type      = b2_dynamicBody;
        b2Body* body = world->CreateBody(&bd);

        b2PolygonShape box;
        box.SetAsBox(10.0f, 0.25f);
        body->CreateFixture(&box, 1.0f);

        b2RevoluteJointDef jd;
        jd.Initialize(ground, body, body->GetPosition());
        jd.lowerAngle  = -8.0f * b2_pi / 180.0f;
        jd.upperAngle  = 8.0f * b2_pi / 180.0f;
        jd.enableLimit = true;
        world->CreateJoint(&jd);

        body->ApplyAngularImpulse(100.0f, true);
    }

    // Bridge
    {
        int32          N = 20;
        b2PolygonShape shape;
        shape.SetAsBox(1.0f, 0.125f);

        b2FixtureDef fd;
        fd.shape    = &shape;
        fd.density  = 1.0f;
        fd.friction = 0.6f;

        b2RevoluteJointDef jd;

        b2Body* prevBody = ground;
        for(int32 i = 0; i < N; ++i) {
            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(161.0f + 2.0f * i, -0.125f);
            b2Body* body = world->CreateBody(&bd);
            body->CreateFixture(&fd);

            b2Vec2 anchor(160.0f + 2.0f * i, -0.125f);
            jd.Initialize(prevBody, body, anchor);
            world->CreateJoint(&jd);

            prevBody = body;
        }

        b2Vec2 anchor(160.0f + 2.0f * N, -0.125f);
        jd.Initialize(prevBody, ground, anchor);
        world->CreateJoint(&jd);
    }

    // Boxes
    {
        b2PolygonShape box;
        box.SetAsBox(0.5f, 0.5f);

        b2Body*   body = NULL;
        b2BodyDef bd;
        bd.type = b2_dynamicBody;

        for(int32 i = 0; i < 5; ++i) {
            bd.position.Set(230.0f, 0.5f + static_cast< float >(i));
            body = world->CreateBody(&bd);
            body->CreateFixture(&box, 0.5f);
            auto e = m_gc->CreateEntity();
            e.AddComponent< PhysicsComponent >(body);
            e.AddComponent< SpriteComponent >(m_gc->GetSpriteInfo("TEST_BOX_0"), b2Vec2(1.0f, 1.0f), b2Vec2(-0.5f, 0.5f));
        }
    }

    // Car
    {
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
        m_car = world->CreateBody(&bd);
        m_car->CreateFixture(&chassis, 1.0f);

        b2FixtureDef fd;
        fd.shape    = &circle;
        fd.density  = 1.0f;
        fd.friction = 0.9f;

        bd.position.Set(-1.0f, 0.35f);
        m_wheel1 = world->CreateBody(&bd);
        m_wheel1->CreateFixture(&fd);

        bd.position.Set(1.0f, 0.4f);
        m_wheel2 = world->CreateBody(&bd);
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
        m_spring1           = ( b2WheelJoint* )world->CreateJoint(&jd);

        jd.Initialize(m_car, m_wheel2, m_wheel2->GetPosition(), axis);
        jd.motorSpeed       = 0.0f;
        jd.maxMotorTorque   = 10.0f;
        jd.enableMotor      = false;
        jd.stiffness        = mass2 * omega * omega;
        jd.damping          = 2.0f * mass2 * dampingRatio * omega;
        jd.lowerTranslation = -0.25f;
        jd.upperTranslation = 0.25f;
        jd.enableLimit      = true;
        m_spring2           = ( b2WheelJoint* )world->CreateJoint(&jd);

        auto e_chassis = m_gc->CreateEntity();
        e_chassis.AddComponent< PhysicsComponent >(m_car);
        e_chassis.AddComponent< SpriteComponent >(m_gc->GetSpriteInfo("CAR_CHASSIS"), b2Vec2(3.f, 1.4f), b2Vec2(-1.5f, 0.9f));
        e_chassis.AddComponent< TagComponent >("car chassis");
        e_chassis.AddComponent<DebugLayerComponent>(std::make_unique< CarDebugLayer >(e_chassis));
        e_chassis.AddComponent<PlayerComponent>();

        auto e_wheel_front = m_gc->CreateEntity();
        e_wheel_front.AddComponent< PhysicsComponent >(m_wheel1);
        e_wheel_front.AddComponent< SpriteComponent >(m_gc->GetSpriteInfo("CAR_WHEEL"), b2Vec2(0.8f, 0.8f), b2Vec2(-0.4f, 0.4f));

        auto e_wheel_back = m_gc->CreateEntity();
        e_wheel_back.AddComponent< PhysicsComponent >(m_wheel2);
        e_wheel_back.AddComponent< SpriteComponent >(m_gc->GetSpriteInfo("CAR_WHEEL"), b2Vec2(0.8f, 0.8f), b2Vec2(-0.4f, 0.4f));
    }
}

bool Car::HandleKeyboard(SDL_Keycode key) {
    switch(key) {
        CC_INFO("Key pressed: {}", key);
    // case SDLK_SPACE: {
    //     // m_chassis->ApplyForce(b2Vec2(0.0f, m_JumpForce), b2Vec2_zero, true);
    //     // break;
    // }
    case SDLK_a: {
        CC_INFO("Key [a] pressed");

        m_spring1->SetMotorSpeed(m_speed);
        break;
    }

    case SDLK_s: {
        m_spring1->SetMotorSpeed(0.0f);
        break;
    }

    case SDLK_d: {
        m_spring1->SetMotorSpeed(-m_speed);
        break;
    }

    default: {
        return false;
    }
    }
    return true;
}
