#pragma once

#include "Core/Settings.h"
#include "Util/imgui_include.h"
#include "UI/Layer.h"

class ExampleLayer : public Layer {
    public: 
    ExampleLayer(Settings& settings) : m_settings(settings) {}

    void GuiRender() override {
        {
            // test demo windon
            if(m_settings.m_showDemoWindow)
                ImGui::ShowDemoWindow();
        }
    }
    Settings& m_settings;
};