#pragma once

#include <SDL2/SDL_keycode.h>
#include <box2d/box2d.h>

#include "Coconut.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "imgui.h"

// A TheJansen Loader
class Car {
public:
    Car(GameContext* gc);

    bool HandleKeyboard(SDL_Keycode key);

private:
    b2Body* m_car;
    b2Body* m_wheel1;
    b2Body* m_wheel2;

    float         m_speed;
    b2WheelJoint* m_spring1;
    b2WheelJoint* m_spring2;

    GameContext* m_gc;

    friend class CarDebugLayer;
};

class CarDebugLayer : public Layer {
public:
    CarDebugLayer(Entity car) : m_car(car) {}
    void GuiRender() override {
        ImGui::Begin("Car Debug");
        ImGui::SeparatorText("Sprite Positioning");
        
        auto& sprite  = m_car.GetComponent< SpriteComponent >();
        auto& physics = m_car.GetComponent< PhysicsComponent >();

        auto body_pos = physics.GetBody()->GetPosition();
        ImGui::Text("m_body position: %.2f %.2f", body_pos.x, body_pos.y);

        b2Vec2 box_size = sprite.GetBoxSize();
        ImGui::SliderFloat("h", &box_size.x, 0.0f, 5.0f);
        ImGui::SliderFloat("w", &box_size.y, 0.0f, 5.0f);
        sprite.SetBoxSize(box_size);

        b2Vec2 offset = sprite.GetLocalOffset();
        ImGui::SliderFloat("offset.x", &offset.x, -5.0f, 5.0f);
        ImGui::SliderFloat("offset.y", &offset.y, -5.0f, 5.0f);
        sprite.SetOffset(offset);

        ImGui::SeparatorText("Car Movement");
        ImGui::End();
    }

private:
    Entity m_car;
};
