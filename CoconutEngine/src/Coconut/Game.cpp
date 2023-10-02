// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "Coconut/Game.h"

#include "Coconut/TextureManager.h"
#include "Coconut/Log.h"
#include "Coconut/GameMap/Map.h"


#include "Coconut/ECS/Compoent.h"
#include "Coconut/Vector2D.h"
#include "Coconut/Collision.h"
#include "Coconut/ECS/ColliderComponent.h"

std::unique_ptr<Coconut::Map> map;

// static value initialize
SDL_Renderer* Coconut::Game::renderer = nullptr;
SDL_Event Coconut::Game::event;

// global variables
Coconut::Manager manager;
//Coconut::Entity& player(manager.addEntity());
auto& player = manager.addEntity();
//Coconut::Entity& wall(manager.addEntity());
auto& dirt = manager.addEntity();

Coconut::Game::Game() {
	m_gameCounter = 0;

	// This is set in gameInit();
	m_isRunning = false;
	// This is set in gameInit();
	m_window = nullptr;
}

Coconut::Game::~Game() {}

void Coconut::Game::gameInit(std::string title, int x, int y, int width, int height, bool fullscreen) {
	int flag = 0;
	if (fullscreen) {
		flag = SDL_WINDOW_FULLSCREEN;
	}

	// SDL Successfully initialized
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
	// SDL failed to initialize
	else {
		CC_CORE_ERROR("SDL failed to initialized.");
		m_isRunning = false;
	}

	Coconut::TextureManager::showFileInfo();
	map = std::make_unique<Coconut::Map>();


	//Allocate instances

	player.addComponent<Coconut::TransformComponent>();
	player.addComponent<Coconut::SpriteComponent>("bird.png");
	player.addComponent<Coconut::KeyboardController>();
	player.addComponent<Coconut::ColliderComponent>("player");
	
	dirt.addComponent<Coconut::TransformComponent>(100, 100, 0, 1);
	dirt.addComponent<Coconut::SpriteComponent>("dirt.jpg");
	dirt.addComponent<Coconut::ColliderComponent>("dirt");
}

void Coconut::Game::handleEvents() {
	SDL_PollEvent(&Coconut::Game::event);

	switch (Coconut::Game::event.type) {
	case SDL_QUIT:
		m_isRunning = false;
		break;
	default:
		break;
	}

}

void Coconut::Game::update() {
	// call obj's update function from here

	manager.refresh();
	manager.update(); 

	if (Coconut::Collision::circle(player, dirt)) {
		CC_CORE_INFO("Collision detected!");
	}
}

void Coconut::Game::render() {
	SDL_RenderClear(renderer);

	map->drawMap();
	manager.draw();
	
	SDL_RenderPresent(renderer);
}

void Coconut::Game::clean() {
	SDL_DestroyWindow(m_window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();
	CC_CORE_INFO("Game cleaned!");
}



