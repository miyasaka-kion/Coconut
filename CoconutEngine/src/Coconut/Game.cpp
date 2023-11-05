#include "Coconut/Game.h"

#include <memory>

#include <SDL_error.h>
#include <box2d/box2d.h>

#include "Coconut/ECS/Compoent.h"
#include "Coconut/GameMap/Map.h"
#include "Coconut/Log.h"
#include "Coconut/TextureManager.h"


std::unique_ptr<Coconut::Map> map;

// static value initialize
SDL_Renderer* Coconut::Game::renderer = nullptr;
SDL_Event     Coconut::Game::event;
std::unique_ptr<b2World> Coconut::Game::m_world = nullptr;

// global variables
Coconut::Manager manager;
auto& player = manager.addEntity();
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
    if(fullscreen) {
        flag = SDL_WINDOW_FULLSCREEN;
    }

    // SDL Successfully initialized
    if(SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        CC_CORE_INFO("SDL Successfully initialized!");
        // window
        m_window = SDL_CreateWindow(title.c_str(), x, y, width, height, flag);
        if(m_window) {
            CC_CORE_INFO("SDL window created!");
        }
        else {
            CC_CORE_ERROR("SDL window failed to create. Error: {}", SDL_GetError());
        }
        // renderer
        renderer = SDL_CreateRenderer(m_window, -1, 0);

        if(renderer) {
            if(SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff)) {
                CC_CORE_INFO("SDL_SetRenderDrawColor, Error: {}", SDL_GetError());
            }
        }
        else {
            CC_CORE_ERROR("Renderer failed to create, Error: {}", SDL_GetError());
        }
        m_isRunning = true;
    }
    // SDL failed to initialize
    else {
        CC_CORE_ERROR("SDL failed to initialized. Error: {}", SDL_GetError());
        m_isRunning = false;
    }

    // Check current asset info
    Coconut::TextureManager::showFileInfo();

    // map init
    map = std::make_unique<Coconut::Map>();

    bool init_physics = true;
    // physics engine init
    if (init_physics) {
        auto gravity = b2Vec2(0.0f, -10.0f);
        m_world = std::make_unique< b2World >(gravity);
    }
        

    player.addComponent<Coconut::TransformComponent>();
    player.addComponent<Coconut::SpriteComponent>("bird.png");
    player.addComponent<Coconut::KeyboardController>();

    dirt.addComponent<Coconut::TransformComponent>(b2Vec2{1.0f, 1.0f}, 0.0f);
    dirt.addComponent<Coconut::SpriteComponent>("dirt.jpg");
}

void Coconut::Game::handleEvents() {
    SDL_PollEvent(&Coconut::Game::event);

    switch(Coconut::Game::event.type) {
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
