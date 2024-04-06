#include "Game.h"

#include <SDL2/SDL_keycode.h>

#include "Coconut.h"

#include "Car.h"



Game::Game() {
    m_game = std::make_unique< GameContext >();
    m_game->RegisterClientHandleEvent([this](SDL_Event& event) -> bool { return ClientHandleEvent(event); });
}

void Game::LoadEntities() {
    m_theo = std::make_unique<Car>(m_game.get());

    // m_game->AddImGuiLayer
    
    // {
    //     Entity edge = m_game->CreateEntity();

    //     b2BodyDef bd;
    //     bd.type   = b2_staticBody;
    //     auto body = m_game->m_world->CreateBody(&bd);

    //     b2EdgeShape edgeShape;
    //     edgeShape.SetTwoSided(b2Vec2(-40.0f, 0.0f), b2Vec2(200.0f, 0.0f));
    //     auto f = body->CreateFixture(&edgeShape, 0.0f);

    //     f->SetFriction(1.0f);
    // }
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
        if(m_theo->HandleKeyboard(sym)) {
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