#pragma once

#include "UI/Layer.h"
#include "Util/imgui_include.h"

struct PlayerSettings {
    float maxSpeed   = 10.0f;
    float jumpForce  = 10.0f;
    float move_force = 10.0f;
};

class GameContext;
class PlayerSettingsPanel : public Layer {
public:
    PlayerSettingsPanel(PlayerSettings& settings) : m_playerSettings(settings) {}
    
    void GUIRender() override {
        ImGui::Begin("Player Settings");

        ImGui::SliderFloat("Max Speed", &m_playerSettings.maxSpeed, 0.0f, 100.0f);
        ImGui::SliderFloat("Jump Force", &m_playerSettings.jumpForce, 0.0f, 100.0f);
        ImGui::SliderFloat("Move Force", &m_playerSettings.move_force, 0.0f, 100.0f);

        ImGui::End();
    }

private:
    PlayerSettings&  m_playerSettings;
};