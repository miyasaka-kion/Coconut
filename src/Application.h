#pragma once
#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "Box.h"
#include "Edge.h"

class Application {
public:
    Application();
    ~Application();
    
    void run();

    void init();  
    void update();

    // load game ground physics and more..
    void loadBoundary();
    void loadEntity();

private:
    void init_sdl_window();
    void init_sdl_renderer();
    void pollEvents();
    void createBar(float ground_x, float ground_y, b2Vec2 point1, b2Vec2 point2);
    
public: 
    b2World* world;

    // should be an Entity list here, I use one Box instead
    Box box;
    Edge edge;
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
};