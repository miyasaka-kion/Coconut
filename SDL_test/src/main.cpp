#include <iostream>
#include <memory>
#include <Game.h>

//void someStuff() {
//	int init_param = SDL_Init(SDL_INIT_EVERYTHING);
//	if (!init_param) {
//		std::cout << "SDL_Init Success!" << std::endl;
//	}
//
//	SDL_Window* Window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED,
//		SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
//
//	SDL_Renderer* renderer = SDL_CreateRenderer(Window, -1, 0);
//
//	bool isRunning = true;
//
//	while (isRunning) {
//		SDL_Event event;
//
//		while (SDL_PollEvent(&event)) {
//			switch (event.type) {
//			case SDL_QUIT:
//				isRunning = false;
//				break;
//
//			case SDL_MOUSEMOTION:
//				std::cout << "Mouse move dected!" << std::endl;
//			}
//		}
//	}
//
//
//	//Todo: use smart pointer	
//
//	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 / 121);
//
//	SDL_RenderClear(renderer);
//
//	SDL_RenderPresent(renderer);
//
//	SDL_Delay(3000);
//
//	std::cout << "Program exited." << std::endl;
//}

std::shared_ptr<Coconut::Game> game = nullptr;


int main(int argc, char* argv[]) {
	game = std::make_shared<Coconut::Game>();

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->render();
	}

	game->clean();
	

	std::cout << "Hellow World" << std::endl;
	return 0;
}