#pragma once

#include <memory>
#include <SDL2/SDL_keycode.h>

#include "Core/GameContext.h"
#include "Player.h"

class Application {
public:
    Application();

    void Run();
    
    bool ClientHandleEvent(SDL_Event& event);
    bool HandlePlayerInput(SDL_Keycode sym);

private:
    void LoadEntities();

    std::unique_ptr< GameContext > m_game;
    // const SDL_Renderer* m_renderer;

    PlayerSettings m_playerSettings;
};