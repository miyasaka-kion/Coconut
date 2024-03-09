#pragma once

#pragma once

#include <algorithm>
#include <memory>
#include <vector>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <entt.hpp>

#include "Render/DebugDraw.h"
#include "imgui.h"

#include "util/sdl_delete.h"
#include "Core/Camera.h"
#include "Core/Settings.h"
#include "Event/KeyboardEvent.h"
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

class Entity;

class GameContext {
public:
    GameContext();
    virtual ~GameContext();

public:
    void NewFrame();
    void PollAndHandleEvents();
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

    // should this return a const?
    [[nodiscard]] SDL_Renderer* GetRenderer() const {
        return m_sdl_renderer.get();
    }


public: // Entity management
    [[nodiscard]] Entity CreateEntity();
    void DestroyEntity(Entity entity);
    
    void SetBackgroundColor(); // this should be remove
    void ShowDebugDraw();
    void ShowHealthAboveEntity();

private:
    void Init_SDL_Window();
    void Init_SDL_Renderer();
    void Init_Imgui();
    void Init_DebugDraw();
    void Init_Box2D();

private:
    // handle input
public:
    using InputCallback = std::function< bool(SDL_Event&) >;
    void RegisterCoreHandleEvent(InputCallback func);
    void RegisterClientHandleEvent(InputCallback func); 
    
    void DefaultCoreHandleEvent(SDL_Event& event);


    // hmmm... this may be the simplest way?
    InputCallback f_CoreHandleEvent;
    InputCallback f_ClientHandleEvent;

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
    SDL::Window  m_sdl_window;
    SDL::Renderer m_sdl_renderer;

    // Entities
private:
    bool           m_closeGame;
    entt::registry m_reg;

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

    friend class Entity;
};

extern Camera   g_camera;
extern Settings g_settings;

struct ImguiSettings {
    bool show_demo_window    = true;
    bool show_another_window = false;
};