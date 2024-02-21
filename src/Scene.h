#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <box2d/box2d.h>
#include <SDL2/SDL.h>

#include "DebugDraw.h"
#include "Settings.h"
#include "imgui.h"

#include "Box.h"
#include "Edge.h"
#include "Entity.h"
#include "Camera.h"

class Scene {
public:
    Scene();
    virtual ~Scene();

    void Run();

private:
    void Init_SDL_Window();
    void Init_SDL_Renderer();
    void Init_Imgui();
    void Init_DebugDraw();
    void Init_Box2D();

    void LoadEntities();
    void PollEvents();
    void UpdateUI();
    void RenderEntities();
    void RemoveInactive();

// temp functions: TODO
    void LoadBox();
    void LoadEdge();
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
    bool                                     m_closeGame;

    // physics info
private:
    ImVec4 m_clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);    
};

extern Camera g_camera;
extern Settings g_settings;

struct ImguiSettings {
    bool show_demo_window    = true;
    bool show_another_window = false;
};