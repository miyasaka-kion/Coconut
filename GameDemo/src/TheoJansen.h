#pragma once

#include <SDL2/SDL_keycode.h>
#include <box2d/box2d.h>

#include "Coconut.h"
#include "imgui.h"

class GameContext;

// A TheJansen Loader
class TheoJansen {
public:
    TheoJansen(GameContext* gc);

    // void OnStep(Settings& settings) override {
    //     g_debugDraw.DrawString(5, m_textLine, "Keys: left = a, brake = s, right = d, toggle motor = m");
    //     m_textLine += m_textIncrement;
    // }

    bool HandleKeyboard(SDL_Keycode key);
private:
    void SetSpeed(const float speed) {
        m_motorJoint->SetMotorSpeed(speed);
    }

    void AssembleTheo(b2World* m_world);

    void EnableMotor() {
        m_motorJoint->EnableMotor(!m_motorJoint->IsMotorEnabled());
    }

private:
    void CreateLeg(float s, const b2Vec2& wheelAnchor);

    b2Vec2           m_offset;
    b2Body*          m_chassis;
    b2Body*          m_wheel;
    b2RevoluteJoint* m_motorJoint;
    bool             m_motorOn = true;

    // Jansen Settings;
    float m_motorSpeed = 100.0f;
    float m_JumpForce = 500.0f;;
    float m_torque = 100.f;

    GameContext* m_gc;

    friend class JansenDebugLayer;
};

class JansenDebugLayer : public Layer {
public:
    JansenDebugLayer(TheoJansen* theo) : m_theo(theo) {}
    void GuiRender() override {
        ImGui::Begin("Jansen Debug");
        ImGui::Text("Motor: [%.2f, %.2f]", m_theo->m_motorSpeed, m_theo->m_motorOn ? 1.0f : 0.0f);
        ImGui::Checkbox("Motor", &m_theo->m_motorOn);
        ImGui::DragFloat("Speed", &m_theo->m_motorSpeed, 0.1f, -1000.0f, 1000.0f);
        ImGui::DragFloat("Torque", &m_theo->m_torque, 0.1f, -1000.0f, 1000.0f);
        ImGui::DragFloat("JumpForce", &m_theo->m_JumpForce, 0.1f, 0.0f, 1000.0f);
        m_theo->SetSpeed(m_theo->m_motorSpeed);
        ImGui::End();
    }

private:
    TheoJansen* m_theo;
};
