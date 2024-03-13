#pragma once

#include "Core/GameContext.h"
#include "Util/imgui_include.h"

#include "Core/PhysicsInfo.h"
#include "UI/Layer.h"

extern Settings g_settings;

class GameContext;

class PhysicsInfoLayer : public Layer {
public:
    PhysicsInfoLayer(GameContext* game_context, PhysicsInfo& info) : Layer(game_context), m_physicsInfo(info) {}
    void Render() override;

private:
    PhysicsInfo& m_physicsInfo;
};