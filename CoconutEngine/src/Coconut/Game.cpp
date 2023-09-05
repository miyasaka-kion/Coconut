#include "Game.h"

SDL_Texture* playerTexture;
SDL_Rect srcR, destR;

Coconut::Game::Game() {
	m_gameCounter = 0;
}

Coconut::Game::~Game() {}


void Coconut::Game::gameInit(std::string title, int x, int y, int width, int height, bool fullscreen) {
	int flag = NULL;
	if (fullscreen) {
		flag = SDL_WINDOW_FULLSCREEN;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
		CC_CORE_INFO("SDL Successfully initialized!");

		//window 
		m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flag);

		if (m_window) {
			CC_CORE_INFO("Window created!");
		}
		else {
			CC_CORE_ERROR("Window failed to create.");
		}

		//renderer
		m_renderer = SDL_CreateRenderer(m_window, -1, 0);

		if (m_renderer) {
			SDL_SetRenderDrawColor(m_renderer, 0xff, 0xff, 0xff, 0xff);

			CC_CORE_INFO("Renderer created!");
		}


		m_isRunning = true;
	}
	else {
		CC_CORE_ERROR("SDL failed to initialized.");
		m_isRunning = false;
	}

	//playerTexture = Coconut::TextureManager::LoadTexture_withFullPath("S:\\projects\\SDL_test\\SDL_test\\assets\\frame-1.png", m_renderer);
	Coconut::TextureManager::showFileInfo();
	playerTexture = Coconut::TextureManager::LoadTexture("frame-1.png", m_renderer);
}

void Coconut::Game::handleEvents() {
	SDL_Event event;
	SDL_PollEvent(&event);
	
	switch (event.type) {
	case SDL_QUIT:
		m_isRunning = false;
		break;
	default:
		break;
	}
	
}

void Coconut::Game::update() {
	// call obj's update function from here
	m_gameCounter++;
	CC_CORE_INFO("gameCounter is {}", m_gameCounter);

	destR.h = 64;
	destR.w = 64;
	destR.x = m_gameCounter;
}

void Coconut::Game::render() {
	SDL_RenderClear(m_renderer);

	SDL_RenderCopy(m_renderer, playerTexture, NULL, &destR);

	SDL_RenderPresent(m_renderer);
}

void Coconut::Game::clean() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(m_renderer);
	SDL_Quit();
	CC_CORE_INFO("Game cleaned!");
}



