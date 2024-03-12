#pragma once 

#include <memory>

#include "Core/GameContext.h"

class Application {
public:
    Application();

    void Run();

    void LoadEntities();

    bool ClientHandleEvent(SDL_Event& event);

private:
    std::unique_ptr<GameContext> m_game;

    SDL_Renderer* m_renderer;
};