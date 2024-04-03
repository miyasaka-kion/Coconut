#pragma once

#include <SDL2/SDL_keycode.h>
#include <box2d/box2d.h>

#include "Coconut.h"
#include "imgui.h"

class GameContext;

// A TheJansen Loader
class Car {
public:
    Car(GameContext* gc);

    bool HandleKeyboard(SDL_Keycode key);

private:
	b2Body* m_car;
	b2Body* m_wheel1;
	b2Body* m_wheel2;

	float m_speed;
	b2WheelJoint* m_spring1;
	b2WheelJoint* m_spring2;

    GameContext* m_gc;

    friend class JansenDebugLayer;
};

// class JansenDebugLayer : public Layer {
// public:
//     JansenDebugLayer(TheoJansen* theo) : m_theo(theo) {}
//     void GuiRender() override {
//         ImGui::Begin("Jansen Debug");
//         ImGui::Text("Motor: [%.2f, %.2f]", m_theo->m_motorSpeed, m_theo->m_motorOn ? 1.0f : 0.0f);
//         ImGui::Checkbox("Motor", &m_theo->m_motorOn);
//         ImGui::DragFloat("Speed", &m_theo->m_motorSpeed, 0.1f, -1000.0f, 1000.0f);
//         ImGui::DragFloat("Torque", &m_theo->m_torque, 0.1f, -1000.0f, 1000.0f);
//         ImGui::DragFloat("JumpForce", &m_theo->m_JumpForce, 0.1f, 0.0f, 1000.0f);
//         m_theo->SetSpeed(m_theo->m_motorSpeed);
//         ImGui::End();
//     }

// private:
//     TheoJansen* m_theo;
// };
