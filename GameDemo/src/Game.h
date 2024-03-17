#pragma once

#include <memory>
#include <SDL2/SDL_keycode.h>

#include "Coconut.h"
#include "TheoJansen.h"

class Game {
public:
    Game();

    void Run();
    
    bool HandlePlayerInput(SDL_Keycode sym);
    bool ClientHandleEvent(SDL_Event& event);

private:
    void LoadEntities();

    std::unique_ptr< GameContext > m_game;
    std::unique_ptr<TheoJansen> m_theo;
};