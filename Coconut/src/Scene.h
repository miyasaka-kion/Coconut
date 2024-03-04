#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>

#include "DebugDraw.h"
#include "Settings.h"
#include "imgui.h"

#include "Camera.h"
#include "ECS/Entity.h"
#include "MouseEvent.h"

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

private:
    void Init_SDL_Window();
    void Init_SDL_Renderer();
    void Init_Imgui();
    void Init_DebugDraw();
    void Init_Box2D();
    // void Init_EntityManager();

    void SetBackgroundColor();

    void LoadEntities();
    void PollEvents();
    void UpdateUI();
    void Step();
    void RenderEntities();
    void RemoveInactive();

    // mouse
private:
    MouseEvent m_mouse;


public:
    std::unique_ptr< b2World > m_world;

    // SDL members
private:
    SDL_Window*   m_SDL_Window;
    SDL_Renderer* m_SDL_Renderer;
    SDL_Event     m_SDL_Event;

    // Entities
private:
    bool                                     m_closeGame;
    std::unique_ptr<EntityManager> m_entityManager;

    // physics info
private:
    int           m_textLine;
    int           m_textIncrement;
    int           m_pointCount;
    b2MouseJoint* m_mouseJoint;  // how to use this?
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