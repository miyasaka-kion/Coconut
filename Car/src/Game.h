#pragma once

#include <SDL2/SDL_keycode.h>
#include <memory>

#include "Car.h"
#include "Coconut.h"

class Game {
public:
    Game();

    void Run();

    bool HandlePlayerInput(SDL_Keycode sym);
    bool ClientHandleEvent(SDL_Event& event);

private:
    void LoadEntities();

    std::unique_ptr< GameContext > m_game;
    std::unique_ptr< Car >         m_car;
};