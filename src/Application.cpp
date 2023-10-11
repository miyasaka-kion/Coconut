#include <iostream>

#include <SDL2/SDL.h>

#include "Application.h"
#include "Constants.h"
#include "MetricConverter.h"
#include "Utils.h"
#include "Box.h"
#include "Edge.h"

void Application::init_sdl_renderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

Application::Application() {
    init_sdl_window();
    init_sdl_renderer();

    // auto world = std::make_shared<b2Vec2>(b2Vec2(0.0f, 9.81f));
    world = new b2World(b2Vec2(0.0f, 9.81f));  // new b2World(b2Vec2(0.0f, 9.81f));

    // this should be a loop to init all entities
    box = Box(world, renderer);
    edge = Edge(world, renderer);
}

Application::~Application() {
    // box2D delete whole world and free memory
    delete world;

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Application::loadBoundary() {
    float ground_x = -0.0f;
    float ground_y = -0.0f;

    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -3.0f;
    startpoint.y = 2.0;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3;
    endpoint.y = 2.0;

    b2Vec2 delta{ 1, 1 };
    delta *= 0.5f;
    startpoint -= delta;
    endpoint -= delta;

    
}

void Application::loadEntity() {}

void Application::createBar(float ground_x, float ground_y, b2Vec2 point1, b2Vec2 point2) {
    b2BodyDef BarDef;
    BarDef.type = b2_staticBody;
    BarDef.position.Set(ground_x, ground_y);
    BarDef.angle    = 0;
    b2Body* BarBody = world->CreateBody(&BarDef);

    b2EdgeShape BarShape;
    BarShape.SetTwoSided(point1, point2);

    b2FixtureDef BarFixtureDef;
    BarFixtureDef.shape = &BarShape;
    BarBody->CreateFixture(&BarFixtureDef);
}


void Application::init() {
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -3.0f;
    startpoint.y = 2.0;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3.0;
    endpoint.y = 2.0;

    b2EdgeShape edgeShape; 
    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector
}

void Application::run() {
    this->init();
    bool   close_game = false;
    while(close_game != true) {
        b2Vec2 pos   = box.Body->GetPosition();  // Body = Body from box
        float  angle = box.Body->GetAngle();

        angle = MetricConverter::toDegree(angle);

        pollEvents();

        // question box, update x and y destination
        box.box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.box.w / 2;
        box.box.y = ((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.box.h / 2;

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

        // Draw ground platform
        // SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);


        
        box.render();

        

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
        print("SDL window failed to initialize! ");
        exit(1);
    }
    print("SDL window initialized.");
}

void Application::pollEvents() {
    bool close_game = false;
    while(SDL_PollEvent(&event)) {
        if(event.type == SDL_QUIT)
            close_game = true;

        else if(event.key.keysym.sym == SDLK_ESCAPE)
            close_game = true;

        else if(event.key.keysym.sym == SDLK_r) {
            box.Body->SetTransform(b2Vec2(x_box, y_box), angle_box);
            box.Body->SetLinearVelocity(vel);
        }
    }
}