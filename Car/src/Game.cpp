#include "Game.h"

#include <SDL2/SDL_keycode.h>

#include "Coconut.h"

#include "Car.h"
#include "Core/Camera.h"



Game::Game() {
    m_game = std::make_unique< GameContext >();
    m_game->RegisterClientHandleEvent([this](SDL_Event& event) -> bool { return ClientHandleEvent(event); });
}

void Game::LoadEntities() {
    m_car = std::make_unique<Car>(m_game.get());

}

void Game::FocusOnPlayer() {
    if(m_focusOnPlayer) {
        auto view = m_game->GetRegistry().view< PlayerComponent, PhysicsComponent >();
        if(view.size_hint() != 1) {
            CC_ERROR("Too many players to focus!");
            return;
        }
        for(auto [entity, player, physics] : view.each()) {
            g_camera.m_center = physics.GetPosition();
        }
    }
}

/**
 * The Run function runs the application and contains the main game loop.
 */
void Game::Run() {
    CC_INFO("Game Running!");

    // TODO: Serialization process
    LoadEntities();

    // game main loop
    while(m_game->isClosed() != true) {
        m_game->NewFrame();

        m_game->HandleEvent();

        m_game->UpdateUI();

        m_game->SetBackgroundColor();  // hmm.. this should be done in NewFrame?

        // add Logic here ...
        {
            FocusOnPlayer();    
        }

        m_game->ShowDebugDraw();

        m_game->RenderEntities();
   
        m_game->Step();

        m_game->PresentSubmitted();

        m_game->GetWorld()->ClearForces();  // clear forces after stepping
    }
}

bool Game::HandlePlayerInput(SDL_Keycode sym) {
    switch(sym) {
    case SDLK_r: {
        m_game->RemoveAllEntities();
        LoadEntities();
        break;
    }
    // control player move by default.
    case SDLK_SPACE: {
        m_focusOnPlayer = !m_focusOnPlayer;
        break;
    }

    case SDLK_w: {
        // player jump

        break;
    }

    default: {
        return false;
    }
    }  // switch
    return true;
}

bool Game::ClientHandleEvent(SDL_Event& event) {
    switch(event.type) {

    case SDL_KEYDOWN: {
        auto sym = event.key.keysym.sym;
        if(m_car->HandleKeyboard(sym)) {
            CC_INFO("Car handled input: {}", sym);
            break;
        }
        if(HandlePlayerInput(sym)) {
            CC_INFO("Application handled input: {}", sym);
            break;
        }
    }
    default: {
        return false;
    }
    }
    return true;
}