#include "UI/PhysicsInfoLayer.h"

void PhysicsInfoLayer::GuiRender() {
    {
        auto world = m_gameContext->GetWorld();

        // box2d settings
        ImGui::Begin("Box2D Settings");
        ImGui::Text("Camera Settings");
        ImGui::SliderFloat("Zoom Level", &g_camera.m_zoom, 0.0f, 1.0f);
        if(ImGui::Button("Reset Camera")) {
            g_camera.ResetView();
        }

        ImGui::Separator();

        ImGui::Checkbox("show DebugDraw", &g_settings.m_showDebugDraw);
        ImGui::Checkbox("draw Sprites", &g_settings.m_drawSprites);

        ImGui::Text("Physics Settings");
        auto gravity = world->GetGravity();
        ImGui::SliderFloat("gravity.y", &gravity.y, -10.0f, 0.0f);
        world->SetGravity(gravity);

        ImGui::SliderInt("Vel Iters", &g_settings.m_velocityIterations, 0, 50);
        ImGui::SliderInt("Pos Iters", &g_settings.m_positionIterations, 0, 50);
        // ImGui::SliderFloat("Hertz", &g_settings.m_hertz, 5.0f, 120.0f, "%.0f hz");

        ImGui::Separator();
        ImGui::Text("Display Hertz:");
        if(ImGui::Button("30Hz"))
            g_settings.m_hertz = 30.0f;
        ImGui::SameLine();
        if(ImGui::Button("60Hz"))
            g_settings.m_hertz = 60.0f;
        ImGui::SameLine();
        if(ImGui::Button("120Hz"))
            g_settings.m_hertz = 120.0f;

        ImVec2 button_sz = ImVec2(-1, 0);
        if(ImGui::Button("Pause (P)", button_sz)) {
            g_settings.m_pause = !g_settings.m_pause;
        }

        if(ImGui::Button("Single Step (O)", button_sz)) {
            g_settings.m_singleStep = !g_settings.m_singleStep;
        }

        ImGui::Separator();

        ImGui::Checkbox("Sleep", &g_settings.m_enableSleep);
        ImGui::Checkbox("Warm Starting", &g_settings.m_enableWarmStarting);
        ImGui::Checkbox("Time of Impact", &g_settings.m_enableContinuous);
        ImGui::Checkbox("Sub-Stepping", &g_settings.m_enableSubStepping);

        ImGui::Separator();

        ImGui::Checkbox("Shapes", &g_settings.m_drawShapes);
        ImGui::Checkbox("Joints", &g_settings.m_drawJoints);
        ImGui::Checkbox("AABBs", &g_settings.m_drawAABBs);
        ImGui::Checkbox("Contact Points", &g_settings.m_drawContactPoints);
        ImGui::Checkbox("Contact Normals", &g_settings.m_drawContactNormals);
        ImGui::Checkbox("Contact Impulses", &g_settings.m_drawContactImpulse);
        ImGui::Checkbox("Friction Impulses", &g_settings.m_drawFrictionImpulse);
        ImGui::Checkbox("Center of Masses", &g_settings.m_drawCOMs);
        ImGui::Checkbox("Statistics", &g_settings.m_drawStats);
        ImGui::Checkbox("Profile", &g_settings.m_drawProfile);
        ImGui::End();
    }
}