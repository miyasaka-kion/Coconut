#include "Scene.h"

#include <cstdio>
#include <iostream>

#include <SDL2/SDL.h>
#include <memory>
#include <stdexcept>

#include "Box.h"
#include "Constants.h"
#include "Edge.h"
#include "Entity.h"
#include "MetricConverter.h"
#include "Log.h"

Scene::Scene() {
    init_sdl_window();
    init_sdl_renderer();

    world = std::make_unique< b2World >(b2Vec2(0.0f, -10.0f));

    loadEntities();
    closeGame = false;
}

Scene::~Scene() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Scene::init_sdl_renderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        CC_CORE_ERROR("SDL renderer initialization failed!");
        throw std::runtime_error("SDL_Renderer initialized a NULL renderer");
    }
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    CC_CORE_INFO("Current SDL_Renderer: {}", info.name);
}

void Scene::run() {
    // game main loop here
    while(closeGame != true) {
        pollEvents();

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

        refresh();
        // removeInactive(); this is currently unneeded!

        SDL_SetRenderDrawColor(renderer, 88, 88,88, 0);
        SDL_RenderPresent(renderer);

        world->Step(1.0f / 60.0f, 10.0f, 2.0f);  // update
        world->ClearForces();
    }
}

void Scene::init_sdl_window() {
    // SDL_Init begin
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width  = DM.w;
    auto Height = DM.h;

    CC_CORE_INFO("Width of the Screen: {}", Width);
    CC_CORE_INFO("Height of the Screen: {}", Height);

    CC_CORE_INFO("The rendering scale is {}", c_pixelPerMeter);

    window = SDL_CreateWindow("SDL with box2d Game Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, c_screenWidthPix, c_screenHeightPix, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        CC_CORE_ERROR( "SDL window failed to initialize! ");
        throw std::runtime_error("SDL_CreateWindow generate a NULL window");
    }
    CC_CORE_INFO("SDL window successfully initialized." );
}

void Scene::pollEvents() {
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT) {
            closeGame = true;
            CC_CORE_INFO("SDL_QUIT Triggered.");
        }

        else if(event.key.keysym.sym == SDLK_ESCAPE) {
            closeGame = true;
            CC_CORE_INFO("ESC pressed!");
            CC_CORE_INFO("SDL_QUIT Triggered.");
        }

        else if(event.key.keysym.sym == SDLK_r) {
            loadEntities();
            CC_CORE_INFO( "r key pressed");
        }
    }
}

void Scene::refresh() {
    for(const auto& entity : entityList) {
        entity->render();
    }
}

void Scene::loadEntities() {
    // some constants
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -3.0f;
    startpoint.y = -2.0;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3.0;
    endpoint.y = -2.0;
    // constants end

    auto box = std::make_unique< Box >(world.get(), renderer);

    box->init(c_OriginPos, b2Vec2(c_OriginalBoxWidth, c_OriginalBoxHeight), c_OriginalVelocity, c_originalAngle);

    auto edge = std::make_unique< Edge >(world.get(), renderer);
    edge->init(startpoint, endpoint);

    entityList.push_back(std::move(box));
    entityList.push_back(std::move(edge));
}

void Scene::removeInactive() {
    entityList.erase(std::remove_if(std::begin(entityList), std::end(entityList), [](const std::unique_ptr< Entity >& entity) { return !entity->isActive; }), entityList.end());
}