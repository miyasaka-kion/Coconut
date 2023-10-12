#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <box2d/box2d.h>
#include <memory>

#include "Box.h"
#include "Edge.h"

class Application {
public:
    Application();
    ~Application();
    
    void run();

    // load game ground physics and more..
    void loadBoundary();
    void loadEntity();

private:
    void init_sdl_window();
    void init_sdl_renderer();
    void pollEvents();
    void createBar(float ground_x, float ground_y, b2Vec2 point1, b2Vec2 point2);
    
public: 
    std::unique_ptr<b2World> world;

    // should be an Entity list here, I use one Box instead
    std::unique_ptr<Box> box;
    std::unique_ptr<Edge> edge;
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_Event event;
    bool closeGame;
};