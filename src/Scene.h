#pragma once
#include <SDL2/SDL.h>
#include <algorithm>
#include <box2d/box2d.h>
#include <memory>
#include <vector>

#include "imgui.h"

#include "Box.h"
#include "Edge.h"
#include "Entity.h"

// should be a singleton
class Scene {
public:
    // >>>>>>>> API of Scene begin >>>>>>>>>>
    // Initialize the sdl window and sdl renderer,
    // Initialize the world of box2d
    // Load(TODO) and add all entities to the game.
    Scene();
    ~Scene();

    void run();

    // <<<<<<<< API of Scene end <<<<<<<<<<<
private:
    void init_sdl_window();
    void init_sdl_renderer();

    void init_imgui();

    void loadEntities();
    void loadBox();
    void loadCircle();
    void loadEdge();
    void pollEvents();
    void refresh();
    void removeInactive();
    void showControlGUI(b2Vec2& gravity);

public:
    std::unique_ptr< b2World > world;

    // SDL members
private:
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Event     event;

    // Entities
private:
    std::vector< std::unique_ptr< Entity > > entityList;
    bool                                     closeGame;

    // physics info
private:
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    bool   m_boxSleep  = false;
};