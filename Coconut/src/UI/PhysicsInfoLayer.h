#pragma once

#include "Core/GameContext.h"
#include "Util/imgui_include.h"

#include "Core/PhysicsInfo.h"
#include "UI/Layer.h"

extern Settings g_settings;

class GameContext;

class PhysicsInfoLayer : public Layer {
public:
    PhysicsInfoLayer(PhysicsInfo& info) : m_physicsInfo(info) {}
    void GUIRender() override;

private:
    PhysicsInfo& m_physicsInfo;
};