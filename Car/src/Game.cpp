#include "Game.h"

#include <SDL2/SDL_keycode.h>

#include "Coconut.h"

#include "Car.h"
#include "Core/Camera.h"
#include "Tile.h"

Game::Game() {
    m_gc = std::make_unique< GameContext >();
    m_gc->RegisterClientHandleEvent([this](SDL_Event& event) -> bool { return ClientHandleEvent(event); });
}

void Game::LoadEntities() {
    m_car = std::make_unique< Car >(m_gc.get());
}

void Game::FocusOnPlayer() {
    if(m_focusOnPlayer) {
        auto view = m_gc->GetRegistry().view< PlayerComponent, PhysicsComponent >();
        if(view.size_hint() != 1) {
            CC_ERROR("Too many players to focus!");
            return;
        }
        for(auto [entity, player, physics] : view.each()) {
            g_camera.m_center = physics.GetPosition();
        }
    }
}

void Game::RenderTile() {
    auto renderer = m_gc->GetRenderer();

    EdgeTile skyTile{ renderer, m_gc->GetSpriteInfo("BACKGROUND_SKY"), b2Vec2(-20.0f, 12.0f), b2Vec2(160.0f, 12.0f), 20.0f };
    skyTile.Render();
    EdgeTile groundTile{ renderer, m_gc->GetSpriteInfo("BACKGROUND_GROUND"), b2Vec2(-20.0f, 0.0f), b2Vec2(160.0f, 0.0f), 20.0f };
    groundTile.Render();
}

/**
 * The Run function runs the application and contains the main game loop.
 */
void Game::Run() {
    CC_INFO("Game Running!");

    // TODO: Serialization process
    LoadEntities();

    // game main loop
    while(m_gc->isClosed() != true) {
        m_gc->NewFrame();

        m_gc->HandleEvent();

        m_gc->UpdateUI();

        m_gc->SetBackground();  // hmm..  should this be done in NewFrame?

        RenderTile();

        // add Logic here ...
        { FocusOnPlayer(); }

        m_gc->ShowDebugDraw();

        m_gc->RenderEntities();

        m_gc->Step();

        m_gc->PresentSubmitted();

        m_gc->GetWorld()->ClearForces();  // clear forces after stepping
    }
}

bool Game::HandlePlayerInput(SDL_Keycode sym) {
    switch(sym) {
    case SDLK_r: {
        m_gc->RemoveAllEntities();
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