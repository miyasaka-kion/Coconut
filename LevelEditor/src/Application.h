#pragma once 

#include <memory>

#include "Core/GameContext.h"

// Seems that OO design is better? **************************
class Application {
public:
    Application();

    void Run();

    void PollEvents();
private:
    std::unique_ptr<GameContext> m_game;


};