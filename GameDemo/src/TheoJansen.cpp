#include "TheoJansen.h"

#include "ECS/Components.h"
#include "Render/SpriteLoader.h"
#include "UI/Layer.h"
#include "imgui.h"

#include "Coconut.h"
#include <SDL2/SDL_keycode.h>

TheoJansen::TheoJansen(GameContext* gc) : m_gc(gc) {
    AssembleTheo(gc->GetWorld());

    auto e = gc->CreateEntity();
    e.AddComponent< PlayerComponent >();
    e.AddComponent< PhysicsComponent >(m_wheel);

    gc->AddImGuiLayer< JansenDebugLayer >(this);
}

void TheoJansen::AssembleTheo(b2World* world) {

    m_offset.Set(0.0f, 8.0f);

    b2Vec2 pivot(0.0f, 0.8f);

    // Chassis
    {
        b2PolygonShape shape;
        shape.SetAsBox(2.5f, 1.0f);

        b2FixtureDef sd;
        sd.density           = 1.0f;
        sd.shape             = &shape;
        sd.filter.groupIndex = -1;
        b2BodyDef bd;
        bd.type     = b2_dynamicBody;
        bd.position = pivot + m_offset;
        m_chassis   = world->CreateBody(&bd);
        m_chassis->CreateFixture(&sd);        
        auto chassis = m_gc->CreateEntity();
        auto info = m_gc->GetSpriteInfo("MAIN_BODY");
        chassis.AddComponent<SpriteComponent>(info, b2Vec2(10.0f, 10.0f));
        chassis.AddComponent<PhysicsComponent>(m_chassis);
        
    }

    {
        b2CircleShape shape;
        shape.m_radius = 1.6f;

        b2FixtureDef sd;
        sd.density           = 1.0f;
        sd.shape             = &shape;
        sd.filter.groupIndex = -1;
        b2BodyDef bd;
        bd.type     = b2_dynamicBody;
        bd.position = pivot + m_offset;
        m_wheel     = world->CreateBody(&bd);
        m_wheel->CreateFixture(&sd);
    }

    {
        b2RevoluteJointDef jd;
        jd.Initialize(m_wheel, m_chassis, pivot + m_offset);
        jd.collideConnected = false;
        jd.motorSpeed       = m_motorSpeed;
        jd.maxMotorTorque   = 400.0f;
        jd.enableMotor      = m_motorOn;
        m_motorJoint        = ( b2RevoluteJoint* )world->CreateJoint(&jd);
    }

    b2Vec2 wheelAnchor;

    wheelAnchor = pivot + b2Vec2(0.0f, -0.8f);

    CreateLeg(-1.0f, wheelAnchor);
    CreateLeg(1.0f, wheelAnchor);

    m_wheel->SetTransform(m_wheel->GetPosition(), 120.0f * b2_pi / 180.0f);
    CreateLeg(-1.0f, wheelAnchor);
    CreateLeg(1.0f, wheelAnchor);

    m_wheel->SetTransform(m_wheel->GetPosition(), -120.0f * b2_pi / 180.0f);
    CreateLeg(-1.0f, wheelAnchor);
    CreateLeg(1.0f, wheelAnchor);
}

void TheoJansen::CreateLeg(float s, const b2Vec2& wheelAnchor) {
    b2Vec2 p1(5.4f * s, -6.1f);
    b2Vec2 p2(7.2f * s, -1.2f);
    b2Vec2 p3(4.3f * s, -1.9f);
    b2Vec2 p4(3.1f * s, 0.8f);
    b2Vec2 p5(6.0f * s, 1.5f);
    b2Vec2 p6(2.5f * s, 3.7f);

    b2FixtureDef fd1, fd2;
    fd1.filter.groupIndex = -1;
    fd2.filter.groupIndex = -1;
    fd1.density           = 1.0f;
    fd2.density           = 1.0f;

    b2PolygonShape poly1, poly2;

    if(s > 0.0f) {
        b2Vec2 vertices[3];

        vertices[0] = p1;
        vertices[1] = p2;
        vertices[2] = p3;
        poly1.Set(vertices, 3);

        vertices[0] = b2Vec2_zero;
        vertices[1] = p5 - p4;
        vertices[2] = p6 - p4;
        poly2.Set(vertices, 3);
    }
    else {
        b2Vec2 vertices[3];

        vertices[0] = p1;
        vertices[1] = p3;
        vertices[2] = p2;
        poly1.Set(vertices, 3);

        vertices[0] = b2Vec2_zero;
        vertices[1] = p6 - p4;
        vertices[2] = p5 - p4;
        poly2.Set(vertices, 3);
    }

    fd1.shape = &poly1;
    fd2.shape = &poly2;

    b2BodyDef bd1, bd2;
    bd1.type     = b2_dynamicBody;
    bd2.type     = b2_dynamicBody;
    bd1.position = m_offset;
    bd2.position = p4 + m_offset;

    bd1.angularDamping = 10.0f;
    bd2.angularDamping = 10.0f;

    b2Body* body1 = m_gc->GetWorld()->CreateBody(&bd1);
    b2Body* body2 = m_gc->GetWorld()->CreateBody(&bd2);

    body1->CreateFixture(&fd1);
    body2->CreateFixture(&fd2);

    {
        b2DistanceJointDef jd;

        // Using a soft distance constraint can reduce some jitter.
        // It also makes the structure seem a bit more fluid by
        // acting like a suspension system.
        float dampingRatio = 0.5f;
        float frequencyHz  = 10.0f;

        jd.Initialize(body1, body2, p2 + m_offset, p5 + m_offset);
        b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
        m_gc->GetWorld()->CreateJoint(&jd);

        jd.Initialize(body1, body2, p3 + m_offset, p4 + m_offset);
        b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
        m_gc->GetWorld()->CreateJoint(&jd);

        jd.Initialize(body1, m_wheel, p3 + m_offset, wheelAnchor + m_offset);
        b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
        m_gc->GetWorld()->CreateJoint(&jd);

        jd.Initialize(body2, m_wheel, p6 + m_offset, wheelAnchor + m_offset);
        b2LinearStiffness(jd.stiffness, jd.damping, frequencyHz, dampingRatio, jd.bodyA, jd.bodyB);
        m_gc->GetWorld()->CreateJoint(&jd);
    }

    {
        b2RevoluteJointDef jd;
        jd.Initialize(body2, m_chassis, p4 + m_offset);
        m_gc->GetWorld()->CreateJoint(&jd);
    }
}

bool TheoJansen::HandleKeyboard(SDL_Keycode key) {
    switch(key) {
        CC_INFO("Key pressed: {}", key);
    case SDLK_SPACE: {
        m_chassis->ApplyForce(b2Vec2(0.0f, m_JumpForce), b2Vec2_zero, true);
        break;
    }
    case SDLK_a: {
        CC_INFO("Key [a] pressed");
        
        if(m_motorSpeed > 0) {
            m_motorSpeed = -m_motorSpeed;
            m_motorJoint->SetMotorSpeed(m_motorSpeed);
        }
        break;
    }

    case SDLK_s: {
        m_motorJoint->SetMotorSpeed(0.0f);
        break;
    }

    case SDLK_d: {
        if(m_motorSpeed < 0) {
            m_motorSpeed = -m_motorSpeed;
            m_motorJoint->SetMotorSpeed(m_motorSpeed);
        }
        break;
    }

    case SDLK_m: {
        m_motorJoint->EnableMotor(!m_motorJoint->IsMotorEnabled());
        break;
    default:
        return false;
    }
    }
    return true;
}
