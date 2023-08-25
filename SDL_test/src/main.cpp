#include <iostream>
#include <memory>
#include "SDL.h"



int main(int argc, char* argv[]) {
	int init_param = SDL_Init(SDL_INIT_EVERYTHING);
	if (!init_param) {
		std::cout << "SDL_Init Success!" << std::endl;
	}

	SDL_Window* Window = SDL_CreateWindow("title", SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN);
	
	SDL_Renderer* renderer = SDL_CreateRenderer(Window, -1, 0);

	//std::unique_ptr<SDL_Window> Window(SDL_CreateWindow("Mew", SDL_WINDOWPOS_CENTERED,
	//		SDL_WINDOWPOS_CENTERED, 600, 400, SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI));


	//std::unique_ptr<SDL_Renderer> renderer(SDL_CreateRenderer(Window, -1, 0));

	//Todo: use smart pointer	
	
	SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255 / 121);
	
	SDL_RenderClear(renderer);
	
	SDL_RenderPresent(renderer);
	
	SDL_Delay(3000);
	
	std::cout << "Program exited." << std::endl;
	return 0;
}