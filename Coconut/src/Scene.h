#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <entt.hpp>

#include "Render/DebugDraw.h"
#include "Core/Settings.h"
#include "imgui.h"

#include "Core/Camera.h"
#include "Event/MouseEvent.h"
#include "Render/QuadWrite.h"

const int32 k_maxContactPoints = 2048;

struct ContactPoint {
    b2Fixture*   fixtureA;
    b2Fixture*   fixtureB;
    b2Vec2       normal;
    b2Vec2       position;
    b2PointState state;
    float        normalImpulse;
    float        tangentImpulse;
    float        separation;
};

class Scene {
public:
    Scene();
    virtual ~Scene();

    void Run();

public:
    void LoadEntities();
    void PollEvents();
    void UpdateUI();
    void Step();
    void RenderEntities();
    void RemoveInactive();

private:
    void Init_SDL_Window();
    void Init_SDL_Renderer();
    void Init_Imgui();
    void Init_DebugDraw();
    void Init_Box2D();

    void SetBackgroundColor();

public:
    [[nodiscard]] entt::registry& Reg() {
        return m_reg;
    }
    // mouse
private:
    MouseEvent m_mouse;
    // KeyboardEvent ??
public:
    std::unique_ptr< b2World > m_world;

    // SDL members
private:
    SDL_Window*   m_sdl_window;
    SDL_Renderer* m_sdl_renderer;
    SDL_Event     m_sdl_event;

    // Entities
private:
    bool           m_closeGame;
    entt::registry m_reg;  // have a registry per scene

    // physics info
private:
    int           m_textLine;
    int           m_textIncrement;
    int           m_pointCount;
    b2MouseJoint* m_mouseJoint;  // TODO: how to use this?
    b2Profile     m_maxProfile;
    b2Profile     m_totalProfile;
    int           m_stepCount;
    ContactPoint  m_points[k_maxContactPoints];

private:
    ImVec4 m_clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);  // render bg color
};

extern Camera   g_camera;
extern Settings g_settings;

struct ImguiSettings {
    bool show_demo_window    = true;
    bool show_another_window = false;
};