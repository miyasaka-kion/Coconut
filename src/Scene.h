#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <box2d/box2d.h>
#include <SDL2/SDL.h>

#include "imgui.h"

#include "Box.h"
#include "Edge.h"
#include "Entity.h"
#include "Camera.h"



class Scene {
public:
    // >>>>>>>> API of Scene begin >>>>>>>>>>
    // Initialize the sdl window and sdl renderer,
    // Initialize the world of box2d
    // Load(TODO) and add all entities to the game.
    Scene();
    ~Scene();

    void Run();

    // <<<<<<<< API of Scene end <<<<<<<<<<<
private:
    void Init_SDL_Window();
    void Init_SDL_Renderer();

    void Init_imgui();

    void LoadEntities();
    void LoadBox();
    void LoadEdge();
    void PollEvents();
    void UpdateUI();
    void RenderEntities();
    void RemoveInactive();

public:
    std::unique_ptr< b2World > m_world;
    
    // SDL members
private:
    SDL_Window*   m_SDL_Window;
    SDL_Renderer* m_SDL_Renderer;
    SDL_Event     m_SDL_Event;

    // Entities
private:
    std::vector< std::unique_ptr< Entity > > m_entityList;
    bool                                     closeGame;

    // physics info
private:
    ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);    
};

extern Camera g_camera;


struct ImguiSettings {
    bool show_demo_window    = true;
    bool show_another_window = false;
};