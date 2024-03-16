#include "Application.h"

#include <SDL2/SDL_keycode.h>

#include "Core/GameContext.h"
#include "Core/Log.h"
#include "Core/PhysicsInfo.h"
#include "Core/TextureManager.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "ECS/SpriteComponent.h"
#include "Render/SpriteLoader.h"
#include "Util/imgui_include.h"

#include "Player.h"
#include "TheoJansen.h"

Application::Application() {
    m_game = std::make_unique< GameContext >();
    // m_renderer = m_game->GetRenderer();
    m_game->RegisterClientHandleEvent([this](SDL_Event& event) -> bool { return ClientHandleEvent(event); });
    // Regester input callback here, if needed. If not registered the default input callback will be used.

    m_game->CreateGuiLayer< PlayerSettingsPanel >(m_playerSettings);
}

void Application::LoadEntities() {
    TheoJansen theo(m_game.get());
}

/**
 * The Run function runs the application and contains the main game loop.
 */
void Application::Run() {
    CC_INFO("Application Running!");

    // TODO: Serialization process
    LoadEntities();

    // game main loop
    while(m_game->isClosed() != true) {
        // [[maybe_unused]] auto io = ImGui::GetIO();
        // ImGui context begin
        m_game->NewFrame();

        m_game->HandleEvent();

        // ImGui context end

        m_game->UpdateUI();

        m_game->SetBackgroundColor();  // hmm.. this should be done in NewFrame?

        // add Logic here ...

        m_game->RenderEntities();

        m_game->ShowDebugDraw();

        m_game->Step();

        m_game->PresentSubmitted();

        m_game->GetWorld()->ClearForces();  // clear forces after stepping
    }
}

bool Application::HandlePlayerInput(SDL_Keycode sym) {
    switch(sym) {
    case SDLK_r: {
        LoadEntities();
        break;
    }
    // control player move by default.
    case SDLK_SPACE:
        [[fallthrough]];

    case SDLK_w: {
        // player jump

        break;
    }

    case SDLK_d: {

        auto view = m_game->GetRegistry().view< PlayerComponent, PhysicsComponent >();
        for(auto [entity, player, physics] : view.each()) {
            auto body = physics.GetBody();
            body->ApplyForceToCenter(b2Vec2(m_playerSettings.move_force, 0.0f), true);
        }
        break;
    }

    case SDLK_a: {
        // player move left
        auto view = m_game->GetRegistry().view< PlayerComponent, PhysicsComponent >();
        for(auto [entity, player, physics] : view.each()) {
            auto body = physics.GetBody();
            body->ApplyForceToCenter(b2Vec2(-m_playerSettings.move_force, 0.0f), true);
        }
        break;
    }

    case SDLK_s: {
        // player crouch

        break;
    }

    case SDLK_f: {
        // player shoot

        break;
    }

    default: {
        return false;
    }
    }  // switch
    return true;
}

bool Application::ClientHandleEvent(SDL_Event& event) {
    switch(event.type) {

    case SDL_KEYDOWN: {
        auto sym = event.key.keysym.sym;
        if(HandlePlayerInput(sym)) {
            break;
        }
    }
    default: {
        return false;
    }
    }
    return true;
}