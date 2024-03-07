#pragma once

#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <entt.hpp>

#include "Core/Settings.h"
#include "Render/DebugDraw.h"
#include "imgui.h"

#include "Core/Camera.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Render/QuadWrite.h"

const int32 k_maxContactPoints = 2048;
using InputCallback            = std::function< void(SDL_Event&) >;

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

class GameContext {
public:
    GameContext();
    virtual ~GameContext();

public:
    void LoadEntities();

    void NewFrame();
    void PollEvents();
    void UpdateUI();
    void Step();
    void RenderEntities();
    void PresetSubmitted();
    void RemoveInactive();

public:
    [[nodiscard]] const bool isClosed() const {
        return m_closeGame;
    }
    [[nodiscard]] b2World* const GetWorld() const {
        return m_world.get();
    }

    void WindowShouldClose() {
        m_closeGame = true;
    }

public:
    void SetBackgroundColor();
    void ShowDebugDraw();

private:
    void Init_SDL_Window();
    void Init_SDL_Renderer();
    void Init_Imgui();
    void Init_DebugDraw();
    void Init_Box2D();

private:
    [[nodiscard]] entt::registry& Reg() {  // TODO: the m_reg should be integrated into the "EntityManager" in the future;
        return m_reg;
    }

    // handle input
public:
    void RegisterInputCallback(InputCallback func);
    void DefaultInputCallback(SDL_Event& event);

private:
    void CallHandleInput(SDL_Event& event);

    // hmmm... this may be the simplest way?
    InputCallback func_InputCallback;

    // Another way to deal with this:
    // public:
    //     void SetMouse(std::unique_ptr<MouseEvent>& mouseEvent);
    //     void SetKeyboard(std::unique_ptr<KeyboardEvent>& keyboardEvent);
    // private:
    //     std::unique_ptr< MouseEvent >    m_mouse;
    //     std::unique_ptr< KeyboardEvent > m_keyboard;

public:
    std::unique_ptr< b2World > m_world;

    // SDL members
private:
    SDL_Window*   m_sdl_window;
    SDL_Renderer* m_sdl_renderer;

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
    ImVec4 m_clear_color = ImVec4(0.1f, 0.1f, 0.1f, 1.00f);  // render bg color, this is tmp var, should be removed in the future;
};

extern Camera   g_camera;
extern Settings g_settings;

struct ImguiSettings {
    bool show_demo_window    = true;
    bool show_another_window = false;
};