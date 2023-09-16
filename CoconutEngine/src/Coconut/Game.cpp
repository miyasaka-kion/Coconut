// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com


#include "Game.h"
#include "Coconut/TextureManager.h"
#include "Coconut/Log.h"
#include "Coconut/Object.h"
#include "Coconut/GameMap/MapTweaker.h"
#include "Coconut/GameMap/Map.h"


#include "Coconut/ECS/ECS.h"
#include "Coconut/ECS/Compoent.h"

std::shared_ptr<Coconut::Object> player;
std::shared_ptr<Coconut::Object> bird;
std::shared_ptr<Coconut::Map> map;

SDL_Renderer* Coconut::Game::renderer = nullptr;

Coconut::Manager manager;
Coconut::Entity& newPlayter(manager.addEntity());



Coconut::Game::Game() {
	m_gameCounter = 0;

	// This is set in gameInit();
	m_isRunning = false;
	// This is set in gameInit();
	m_window = nullptr;
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
		renderer = SDL_CreateRenderer(m_window, -1, 0);

		if (renderer) {
			SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);

			CC_CORE_INFO("Renderer created!");
		}


		m_isRunning = true;
	}
	else {
		CC_CORE_ERROR("SDL failed to initialized.");
		m_isRunning = false;
	}

	Coconut::TextureManager::showFileInfo();
	//playerTexture = Coconut::TextureManager::LoadTexture("frame-1.png", m_renderer);

	//Allocate instances
	player = std::make_shared<Coconut::Object>("bird.png", 0, 0);
	bird = std::make_shared<Coconut::Object>("bird2.png", 40, 40);
	map = std::make_shared<Coconut::Map>();

	newPlayter.addComponent<Coconut::PositionComponent>();
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
	player->objUpdate();
	bird->objUpdate();
	manager.update();

	CC_CORE_INFO("player position: ({}, {})", newPlayter.getComponent<Coconut::PositionComponent>().x(),
		newPlayter.getComponent<Coconut::PositionComponent>().y());
}

void Coconut::Game::render() {
	SDL_RenderClear(renderer);

	map->drawMap();
	player->objRender();
	bird->objRender();
	
	SDL_RenderPresent(renderer);
}

void Coconut::Game::clean() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	CC_CORE_INFO("Game cleaned!");
}



