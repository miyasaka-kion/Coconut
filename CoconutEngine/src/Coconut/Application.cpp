#include "Application.h"

#include "Coconut/ConstantSpec.h"

namespace Coconut {
Application::Application() {}
Application::~Application() {}

void Application::run() {
    // const int FPS        = 60;
    // const int frameDelay = 1000 / FPS;

    // Uint32 frameStart;
    // int    frameTime;

    // init logger
    Coconut::Log::init();
    CC_CORE_WARN("Core logger initialized!");
    // init logger finish 

    // create an instance of the game
    std::shared_ptr<Coconut::Game> game = std::make_shared<Coconut::Game>();

    game->gameInit("CoconutEngine Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Coconut::Constants::WINDOW_WIDTH, Coconut::Constants::WINDOW_HEIGHT, false);

    // init physics engine
    
    
    while(game->running()) {
        // frameStart = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        // frameTime = SDL_GetTicks() - frameStart;
        // if(frameTime < frameDelay) {
        //     SDL_Delay(frameDelay - frameTime);
        // }
        game->step(); 
    }

    game->clean();
    CC_CORE_CRITICAL("Programme end, Hello world!");
}
}  // namespace Coconut