#pragma once
// This is where the whole program start
#include <SDL.h>

#ifdef CC_PLATFORM_WINDOWS


int main(int argc, char* argv[]) {
	//game = std::make_shared<Coconut::Game>();

	const int FPS = 60;
	const int frameDelay = 1000 / FPS;

	Uint32 frameStart;
	int frameTime;


	Coconut::Log::init();
	CC_CORE_WARN("Core logger initialized!");


	std::shared_ptr<Coconut::Game> game = std::make_shared<Coconut::Game>();

	game->gameInit("CoconutEngine Test Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, false);

	while (game->running()) {
		frameStart = SDL_GetTicks();


		game->handleEvents();
		game->update();
		game->render();


		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < frameDelay) {
			SDL_Delay(frameDelay - frameTime);
		}
	}

	game->clean();

	CC_CORE_CRITICAL("Programme end, Hellow world!");
	return 0;
}

#endif