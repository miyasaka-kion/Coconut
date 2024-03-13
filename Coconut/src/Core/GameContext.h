#pragma once

#include <algorithm>
#include <vector>

#include <SDL2/SDL.h>
#include <box2d/box2d.h>
#include <memory>
#include <entt.hpp>

#include "Render/DebugDraw.h"
#include "imgui.h"

#include "Core/Camera.h"
#include "Core/PhysicsInfo.h"
#include "Core/Settings.h"
#include "Core/TextureManager.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "Render/QuadWrite.h"
#include "Render/SpriteLoader.h"
#include "Util/sdl_delete.h"
#include "UI/Layer.h"

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

    // emm.. this should be change and remove in the future..
    [[nodiscard]] const entt::registry& GetRegistry() const {
        return m_reg;
    }

    // template< typename... Args >
    // [[nodiscard]] const entt::view< entt::entity, Args... > GetView() const {
    //     return m_reg.view< Args... >();
    // }

    void WindowShouldClose() {
        m_closeGame = true;
    }

    [[nodiscard]]   SDL_Renderer* GetRenderer() const {
        return m_sdl_renderer.get();
    }

    // // TODO: this should only be callable by SpriteLoader!
    // [[nodiscard]] const SDL_Texture* GetTexture(const std::string& name) const {
    //     return m_textureManager.GetTexture(name);
    // }

    [[nodiscard]] const SpriteInfo& GetSpriteInfo(const std::string& name) const {
        return m_spriteLoader.GetSpriteInfo(name);
    }

public:  // Entity management
    [[nodiscard]] Entity CreateEntity();
    void                 DestroyEntity(Entity entity);

    void SetBackgroundColor();  // this should be remove
    void ShowDebugDraw();
    void ShowHealthBar();

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

    /**
     * Client handle event callback function.
     *
     * This function is called in PollAndHandleEvents(). Return true if the event was handled, false if you want to let Core to handle the event.
     */
    InputCallback f_ClientHandleEvent;

public:
    std::unique_ptr< b2World > m_world;

    // SDL members
private:
    SDL::Window   m_sdl_window;
    SDL::Renderer m_sdl_renderer;

    // Entities
private:
    bool           m_closeGame;
    entt::registry m_reg;

    TextureManager m_textureManager;
    SpriteLoader   m_spriteLoader;

    // debugdraw
    int m_textLine;
    int m_textIncrement;

    // physics info
    PhysicsInfo m_physicsInfo;

    // UI 
public:
    template < typename T, typename... Args >
    void AddUILayer(Args&&... args) {
        m_layerManager.AddLayer<T>(std::forward<Args>(args)...);
    }
    
private:
    LayerManager m_layerManager;

private:

    friend class Entity;
    friend class Layer;
};

extern Camera   g_camera;
extern Settings g_settings;

struct ImguiSettings {
    bool show_demo_window    = true;
    bool show_another_window = false;
};