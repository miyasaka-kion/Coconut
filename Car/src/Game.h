#pragma once

#include <SDL2/SDL_keycode.h>
#include <memory>

#include "Car.h"
#include "Coconut.h"
#include "Tile.h"

class Game {
public:
    Game();

    void Run();
    bool ClientHandleEvent(SDL_Event& event);


private:
    void LoadEntities();
    bool HandlePlayerInput(SDL_Keycode sym);
    void FocusOnPlayer();
    void RenderTile();

private:
    std::unique_ptr< GameContext > m_gc;
    std::unique_ptr< Car >         m_car;

    bool m_focusOnPlayer = false;


};