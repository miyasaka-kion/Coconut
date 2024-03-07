#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Core/GameContext.h"
#include "Core/Log.h"

Application::Application() {
    m_game = std::make_unique< GameContext >();
    // Regester input callback here, if needed. If not registered the default input callback will be used.
}


/**
 * The Run function runs the application and contains the main game loop.
 */
void Application::Run() {
    CC_INFO("Application Running!");

    // game main loop
    while(m_game->isClosed() != true) {
        // [[maybe_unused]] auto io = ImGui::GetIO();
        // ImGui context begin
        m_game->NewFrame();

        m_game->PollEvents();
        this->PollEvents();

        m_game->UpdateUI();

        m_game->SetBackgroundColor();  // hmm.. this should be done in NewFrame?

        // add Logic here ...

        m_game->RenderEntities();

        {
            // some SDL draw test
        }

        m_game->ShowDebugDraw();

        m_game->Step();

        m_game->PresetSubmitted();

        m_game->GetWorld()->ClearForces();  // clear forces after stepping
    }
}

void Application::PollEvents() {}
