#pragma once

#include <SDL2/SDL_keycode.h>
#include <memory>

#include "Car.h"
#include "Coconut.h"

class Game {
public:
    Game();

    void Run();
    bool ClientHandleEvent(SDL_Event& event);


private:
    void LoadEntities();
    bool HandlePlayerInput(SDL_Keycode sym);
    void FocusOnPlayer();

    std::unique_ptr< GameContext > m_game;
    std::unique_ptr< Car >         m_car;

    bool m_focusOnPlayer = false;
};