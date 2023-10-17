#include <cstdio>
#include <iostream>

#include <SDL2/SDL.h>
#include <memory>
#include <stdexcept>

#include "Application.h"
#include "Box.h"
#include "Constants.h"
#include "Edge.h"
#include "MetricConverter.h"
#include "Utils.h"



Application::Application() {
    init_sdl_window();
    init_sdl_renderer();

    world = std::make_unique<b2World>(b2Vec2(0.0f, -10.0f));

    // this should be a loop to init all entities
    box  = std::make_unique<Box> (world.get(), renderer);
    edge = std::make_unique<Edge> (world.get(), renderer);
    
    
    closeGame = false;
}

Application::~Application() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::init_sdl_renderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        std::cout << "SDL renderer initialization failed!" << std::endl;
        throw std::runtime_error("SDL_Renderer initialized a NULL renderer");
    }
    std::cout << "SDL renderer initialized!" << std::endl;
}

void Application::run() {
    // game main loop here
    while(closeGame != true) {
        pollEvents();

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

        box->render();
        edge->render();
        // refresh();
        
        SDL_SetRenderDrawColor(renderer, 32, 70, 49, 0);
        SDL_RenderPresent(renderer);

        world->Step(1.0f / 60.0f, 6.0f, 2.0f);  // update
    }
}

void Application::init_sdl_window() {
    // SDL_Init begin
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width  = DM.w;
    auto Height = DM.h;

    std::cout << "Width of the Screen: " << Width << std::endl;
    std::cout << "Height of the Screen: " << Height << std::endl;

    window = SDL_CreateWindow("SDL with box2d Game Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        std::cout<< "SDL window failed to initialize! " << std::endl;
        throw std::runtime_error("SDL_CreateWindow generate a NULL window");
    }
    std::cout << "SDL window successfully initialized." << std::endl;
}

void Application::pollEvents() {
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            closeGame = true;
            std::cout << "SDL_QUIT Triggered." << std::endl;
        }

        else if(event.key.keysym.sym == SDLK_ESCAPE) {
            closeGame = true;
            std::cout << "SDL_QUIT Triggered." << std::endl; 
            std::cout << "ESC pressed!" << std::endl;
        }
            
        else if(event.key.keysym.sym == SDLK_r) {
            box->body->SetTransform(b2Vec2(x_box, y_box), angle_box);
            box->body->SetLinearVelocity(vel);
            std::cout << "r key pressed" << std::endl;
        }
    }
}

void Application::refresh() {
    for(const auto& entity : entityList) {
        entity->render();
    }
}