#include "Game.h"

Coconut::Game::Game() {}

Coconut::Game::~Game() {}


void Coconut::Game::gameInit(const char* title, int x, int y, int width, int height, bool fullscreen) {

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		Coconut::Log::getLogger()->dump("Successfully initialized!");
		
	}
	
}

void Coconut::Game::handleEvents() {
}

void Coconut::Game::update() {
}

void Coconut::Game::render() {
}

void Coconut::Game::clean() {
}

bool Coconut::Game::running() {
	return false;
}

