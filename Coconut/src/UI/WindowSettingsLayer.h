#pragma once

#include "UI/Layer.h"
#include "Util/imgui_include.h"
#include "Core/Settings.h"
#include "imgui.h"

class WindowSettingsLayer : public Layer {
public: 
     WindowSettingsLayer(Settings& settings) : m_settings(settings) {}
     void Update() override {
         
     }
    void GUIRender() override {
        
        {
            auto& io = ImGui::GetIO();
            ImGui::Begin("Window Settings");                                    // Create a window called "Hello, world!" and append into it.
            ImGui::Checkbox("Demo Window", &m_settings.m_showDemoWindow);  // Edit bools storing our window open/close state
            ImGui::ColorEdit3("bg color", ( float* )&m_settings.m_clear_color);            // Edit 3 floats representing a color

            if(ImGui::Button("clear Entities")) {
                // m_entityManager->ClearEntities();
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
    }
    private:
    Settings& m_settings;
};