#include "Core/GameContext.h"

#include <stdexcept>

#include <SDL2/SDL_timer.h>
#include <SDL_Image.h>
#include <box2d/box2d.h>

#include "Core/Assert.h"
#include "Core/Log.h"
#include "Core/TextureManager.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "Event/MouseEvent.h"
#include "Render/DebugDraw.h"
#include "Render/SpriteLoader.h"
#include "UI/ExampleLayer.h"
#include "UI/Layer.h"
#include "UI/PhysicsInfoLayer.h"
#include "UI/WindowSettingsLayer.h"
#include "Util/imgui_include.h"
#include "Util/sdl_check.h"
#include "Util/sdl_delete.h"

extern Camera g_camera;
Settings      g_settings;

GameContext::GameContext() : m_layerManager(this) {
    Coconut::Log::init();
    Init_SDL_Window();
    Init_SDL_Renderer();
    Init_Imgui();
    Init_Box2D();
    Init_DebugDraw();  // Notice: This should not init in release version

    m_textureManager.Load(std::string(COCONUT_ASSET_PATH), m_sdl_renderer.get());
    m_spriteLoader.Load(COCONUT_ASSET_PATH, &m_textureManager);
    m_closeGame = false;

    AddImGuiLayer< PhysicsInfoLayer >(m_physicsInfo);
    AddImGuiLayer< ExampleLayer >(g_settings);
    AddImGuiLayer< WindowSettingsLayer >(g_settings);
    AddImGuiLayer< TextureManagerDebugLayer >(m_textureManager);
    AddImGuiLayer< SpriteLoaderDebugLayer >(m_spriteLoader);
}

GameContext::~GameContext() {
    IMG_Quit();
    SDL_Quit();
}

void GameContext::Init_SDL_Window() {
    // SDL_Init begin
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        CC_CORE_ERROR("SDL_Init error: {}", SDL_GetError());
    }

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width  = DM.w;
    auto Height = DM.h;

    CC_CORE_INFO("Width of current Screen: {}", Width);
    CC_CORE_INFO("Height of current Screen: {}", Height);

    // CC_CORE_INFO("The rendering scale is {} pixels per meter. (px/1.0f)", c_pixelPerMeter);
    // TODO: Display pixel per meter in UI: statistics

    g_camera.m_width  = g_settings.m_windowWidth;
    g_camera.m_height = g_settings.m_windowHeight;

    m_sdl_window = SDL::Window(SDL_CreateWindow("Coconut Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_camera.m_width, g_camera.m_height, SDL_WINDOW_SHOWN));

    if(m_sdl_window == nullptr) {
        CC_CORE_ERROR("SDL window failed to initialize! ");
        throw std::runtime_error("SDL_CreateWindow generate a NULL window");
    }

    CC_CORE_INFO("SDL window successfully initialized.");
}

void GameContext::Init_SDL_Renderer() {
    // m_SDL_Renderer = SDL_CreateRenderer(m_SDL_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    m_sdl_renderer = SDL::Renderer(SDL_CHECK(SDL_CreateRenderer(m_sdl_window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)));

    SDL_RendererInfo info;
    SDL_GetRendererInfo(m_sdl_renderer.get(), &info);
    CC_CORE_INFO("Current SDL_Renderer: {}", info.name);
}

void GameContext::Init_Imgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ( void )io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;      // Enable docking functionalities
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(m_sdl_window.get(), m_sdl_renderer.get());
    ImGui_ImplSDLRenderer2_Init(m_sdl_renderer.get());
}

void GameContext::Init_Box2D() {
    // physics info initialize
    auto gravity = b2Vec2(0.0f, -10.0f);
    m_world      = std::make_unique< b2World >(gravity);

    m_textLine      = 5;
    m_textIncrement = 18;

    // init physics info
    m_physicsInfo.m_mouseJoint = NULL;
    m_physicsInfo.m_pointCount = 0;
}

void GameContext::Init_DebugDraw() {
    m_debugDraw.Init(m_sdl_renderer.get());
    m_world->SetDebugDraw(&m_debugDraw);

    uint32 flags = 0;
    flags += g_settings.m_drawShapes * b2Draw::e_shapeBit;
    flags += g_settings.m_drawJoints * b2Draw::e_jointBit;
    flags += g_settings.m_drawAABBs * b2Draw::e_aabbBit;
    flags += g_settings.m_drawCOMs * b2Draw::e_centerOfMassBit;
    m_debugDraw.SetFlags(flags);
}

/**
 * Updates the UI for the game context.
 */
void GameContext::UpdateUI() {
    [[maybe_unused]] ImGuiIO& io = ImGui::GetIO();

    auto pw = b2Vec2(0.0f, 0.0f);
    auto ps = g_camera.ConvertWorldToScreen(pw);

    m_layerManager.Update();
    m_layerManager.Render();

    m_textLine += m_textIncrement;
}

void GameContext::Step() {
    float timeStep = g_settings.m_hertz > 0.0f ? 1.0f / g_settings.m_hertz : float(0.0f);
    // float timeStep = 1 / 60.0f;

    if(g_settings.m_pause) {
        if(g_settings.m_singleStep) {
            g_settings.m_singleStep = 0;
        }
        else {
            timeStep = 0.0f;
        }

        m_debugDraw.DrawString(5, m_textLine, "****PAUSED****");
        m_textLine += m_textIncrement;
    }

    uint32 flags = 0;
    flags += g_settings.m_drawShapes * b2Draw::e_shapeBit;
    flags += g_settings.m_drawJoints * b2Draw::e_jointBit;
    flags += g_settings.m_drawAABBs * b2Draw::e_aabbBit;
    flags += g_settings.m_drawCOMs * b2Draw::e_centerOfMassBit;
    m_debugDraw.SetFlags(flags);

    m_world->SetAllowSleeping(g_settings.m_enableSleep);
    m_world->SetWarmStarting(g_settings.m_enableWarmStarting);
    m_world->SetContinuousPhysics(g_settings.m_enableContinuous);
    m_world->SetSubStepping(g_settings.m_enableSubStepping);

    m_physicsInfo.m_pointCount = 0;

    m_world->Step(timeStep, g_settings.m_velocityIterations, g_settings.m_positionIterations);

    // m_world->DebugDraw();

    if(timeStep > 0.0f) {
        ++m_physicsInfo.m_stepCount;
    }

    if(g_settings.m_drawStats) {
        // FPS
        m_debugDraw.DrawString(5, m_textLine, "FPS: %5.2f", 1.0f / timeStep);
        m_textLine += m_textIncrement;

        // Camera
        m_debugDraw.DrawString(5, m_textLine, "Camera Pos: (%4.2f,%4.2f,zoom: %4.2f)", g_camera.m_center.x, g_camera.m_center.y, g_camera.m_zoom);
        m_textLine += m_textIncrement;

        // Mouse
        // handle mouse input
        int xs, ys;
        SDL_GetMouseState(&xs, &ys);
        auto ps = b2Vec2(xs, ys);

        b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);

        m_debugDraw.DrawString(5, m_textLine, "Mouse Pos: (%4.2f,%4.2f)", pw.x, pw.y);
        m_textLine += m_textIncrement;

        // world info
        int32 bodyCount    = m_world->GetBodyCount();
        int32 contactCount = m_world->GetContactCount();
        int32 jointCount   = m_world->GetJointCount();
        m_debugDraw.DrawString(5, m_textLine, "bodies/contacts/joints = %d/%d/%d", bodyCount, contactCount, jointCount);
        m_textLine += m_textIncrement;

        int32 proxyCount = m_world->GetProxyCount();
        int32 height     = m_world->GetTreeHeight();
        int32 balance    = m_world->GetTreeBalance();
        float quality    = m_world->GetTreeQuality();
        m_debugDraw.DrawString(5, m_textLine, "proxies/height/balance/quality = %d/%d/%d/%g", proxyCount, height, balance, quality);
        m_textLine += m_textIncrement;
    }

    // Track maximum profile times
    {
        const b2Profile& p                       = m_world->GetProfile();
        m_physicsInfo.m_maxProfile.step          = b2Max(m_physicsInfo.m_maxProfile.step, p.step);
        m_physicsInfo.m_maxProfile.collide       = b2Max(m_physicsInfo.m_maxProfile.collide, p.collide);
        m_physicsInfo.m_maxProfile.solve         = b2Max(m_physicsInfo.m_maxProfile.solve, p.solve);
        m_physicsInfo.m_maxProfile.solveInit     = b2Max(m_physicsInfo.m_maxProfile.solveInit, p.solveInit);
        m_physicsInfo.m_maxProfile.solveVelocity = b2Max(m_physicsInfo.m_maxProfile.solveVelocity, p.solveVelocity);
        m_physicsInfo.m_maxProfile.solvePosition = b2Max(m_physicsInfo.m_maxProfile.solvePosition, p.solvePosition);
        m_physicsInfo.m_maxProfile.solveTOI      = b2Max(m_physicsInfo.m_maxProfile.solveTOI, p.solveTOI);
        m_physicsInfo.m_maxProfile.broadphase    = b2Max(m_physicsInfo.m_maxProfile.broadphase, p.broadphase);

        m_physicsInfo.m_totalProfile.step += p.step;
        m_physicsInfo.m_totalProfile.collide += p.collide;
        m_physicsInfo.m_totalProfile.solve += p.solve;
        m_physicsInfo.m_totalProfile.solveInit += p.solveInit;
        m_physicsInfo.m_totalProfile.solveVelocity += p.solveVelocity;
        m_physicsInfo.m_totalProfile.solvePosition += p.solvePosition;
        m_physicsInfo.m_totalProfile.solveTOI += p.solveTOI;
        m_physicsInfo.m_totalProfile.broadphase += p.broadphase;
    }

    if(g_settings.m_drawProfile) {
        const b2Profile& p = m_world->GetProfile();

        b2Profile aveProfile;
        memset(&aveProfile, 0, sizeof(b2Profile));
        if(m_physicsInfo.m_stepCount > 0) {
            float scale              = 1.0f / m_physicsInfo.m_stepCount;
            aveProfile.step          = scale * m_physicsInfo.m_totalProfile.step;
            aveProfile.collide       = scale * m_physicsInfo.m_totalProfile.collide;
            aveProfile.solve         = scale * m_physicsInfo.m_totalProfile.solve;
            aveProfile.solveInit     = scale * m_physicsInfo.m_totalProfile.solveInit;
            aveProfile.solveVelocity = scale * m_physicsInfo.m_totalProfile.solveVelocity;
            aveProfile.solvePosition = scale * m_physicsInfo.m_totalProfile.solvePosition;
            aveProfile.solveTOI      = scale * m_physicsInfo.m_totalProfile.solveTOI;
            aveProfile.broadphase    = scale * m_physicsInfo.m_totalProfile.broadphase;
        }

        m_debugDraw.DrawString(5, m_textLine, "step [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.step, aveProfile.step, m_physicsInfo.m_maxProfile.step);
        m_textLine += m_textIncrement;
        m_debugDraw.DrawString(5, m_textLine, "collide [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.collide, aveProfile.collide, m_physicsInfo.m_maxProfile.collide);
        m_textLine += m_textIncrement;
        m_debugDraw.DrawString(5, m_textLine, "solve [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solve, aveProfile.solve, m_physicsInfo.m_maxProfile.solve);
        m_textLine += m_textIncrement;
        m_debugDraw.DrawString(5, m_textLine, "solve init [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveInit, aveProfile.solveInit, m_physicsInfo.m_maxProfile.solveInit);
        m_textLine += m_textIncrement;
        m_debugDraw.DrawString(5, m_textLine, "solve velocity [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveVelocity, aveProfile.solveVelocity, m_physicsInfo.m_maxProfile.solveVelocity);
        m_textLine += m_textIncrement;
        m_debugDraw.DrawString(5, m_textLine, "solve position [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solvePosition, aveProfile.solvePosition, m_physicsInfo.m_maxProfile.solvePosition);
        m_textLine += m_textIncrement;
        m_debugDraw.DrawString(5, m_textLine, "solveTOI [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveTOI, aveProfile.solveTOI, m_physicsInfo.m_maxProfile.solveTOI);
        m_textLine += m_textIncrement;
        m_debugDraw.DrawString(5, m_textLine, "broad-phase [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.broadphase, aveProfile.broadphase, m_physicsInfo.m_maxProfile.broadphase);
        m_textLine += m_textIncrement;
    }

    if(g_settings.m_drawContactPoints) {
        const float k_impulseScale = 0.1f;
        const float k_axisScale    = 0.3f;

        for(int32 i = 0; i < m_physicsInfo.m_pointCount; ++i) {
            ContactPoint* point = m_physicsInfo.m_points + i;

            if(point->state == b2_addState) {
                // Add
                m_debugDraw.DrawPoint(point->position, 10.0f, b2Color(0.3f, 0.95f, 0.3f));
            }
            else if(point->state == b2_persistState) {
                // Persist
                m_debugDraw.DrawPoint(point->position, 5.0f, b2Color(0.3f, 0.3f, 0.95f));
            }

            if(g_settings.m_drawContactNormals == 1) {
                b2Vec2 p1 = point->position;
                b2Vec2 p2 = p1 + k_axisScale * point->normal;
                m_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.9f));
            }
            else if(g_settings.m_drawContactImpulse == 1) {
                b2Vec2 p1 = point->position;
                b2Vec2 p2 = p1 + k_impulseScale * point->normalImpulse * point->normal;
                m_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
            }

            if(g_settings.m_drawFrictionImpulse == 1) {
                b2Vec2 tangent = b2Cross(point->normal, 1.0f);
                b2Vec2 p1      = point->position;
                b2Vec2 p2      = p1 + k_impulseScale * point->tangentImpulse * tangent;
                m_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
            }
        }
    }
}

void GameContext::SetBackground() {
    [[maybe_unused]] const auto& io = ImGui::GetIO();
    SDL_RenderSetScale(m_sdl_renderer.get(), io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    // clang-format off
    SDL_SetRenderDrawColor(m_sdl_renderer.get(),
                            static_cast<Uint8>( g_settings.m_clear_color.x * 255.0f),
                            static_cast<Uint8>( g_settings.m_clear_color.y * 255.0f),
                            static_cast<Uint8>( g_settings.m_clear_color.z * 255.0f),
                            static_cast<Uint8>( g_settings.m_clear_color.w * 255.0f));
    // clang-format on
    SDL_RenderClear(m_sdl_renderer.get());

    if(g_settings.m_showBackgroundImage) {
        // tileManager.SubmitRender();
    }
}

/**
 * Function to start a new frame in the game context. This create ImGui context and refresh some settings in DebugDraw.
 */
void GameContext::NewFrame() {
    ImGui_ImplSDLRenderer2_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // debugDraw m_textLine reset
    m_textLine = 26;
}

/**
 * Polls events in the game context and handles mouse and keyboard input.
 */
void GameContext::HandleEvent() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        if(f_ClientHandleEvent == nullptr) {
            CC_CORE_WARN("Client event handle callback is not set!");
        }
        if(!f_ClientHandleEvent(event)) {
            DefaultCoreHandleEvent(event);
        }
    }
}

void GameContext::RenderEntities() {
    if(g_settings.m_drawSprites == false) {
        return;
    }
    auto      view = m_reg.view< PhysicsComponent, SpriteComponent >();  // TODO: temp sol
    QuadWrite writer(m_sdl_renderer.get());
    for(auto [entity, physics, sprite] : view.each()) {
        auto& info = sprite.GetSpriteInfo();
        writer.Render(&info, sprite.GetBoxSize(), physics.GetPosition(), sprite.GetLocalOffset(), physics.GetAngle());
    }

    if(g_settings.m_showDebugEntity) {
        auto view = m_reg.view< DebugLayerComponent >();
        for(auto [entity, debug] : view.each()) {
            debug.GuiRender();
        }
    }
}

void GameContext::RemoveInactive() {
    auto view = m_reg.view< PhysicsComponent >();
    for(auto entity : view) {
        // TODO
    }
}

void GameContext::RemoveAllEntities() {
    m_reg.clear();
}

/**
 * Function called when a preset is submitted to SDL renderer.
 */
void GameContext::PresentSubmitted() {
    ImGui::Render();
    ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
    // ImGui context end
    SDL_RenderPresent(m_sdl_renderer.get());
}

/**
 * Registers an input callback function. If no input callback function is registered, the default input callback is used.
 *
 * @param func The input callback function to register
 *
 * @return void
 *
 * @throws ErrorType None
 */
void GameContext::RegisterCoreHandleEvent(InputCallback func) {
    f_CoreHandleEvent = func;
}

void GameContext::RegisterClientHandleEvent(InputCallback func) {
    f_ClientHandleEvent = func;
}

// the default version of input callback
void GameContext::DefaultCoreHandleEvent(SDL_Event& event) {
    static MouseEvent mouse;
    bool              not_handled = false;

    ImGui_ImplSDL2_ProcessEvent(&event);

    // handle mouse input
    int xs, ys;
    SDL_GetMouseState(&xs, &ys);
    auto ps = b2Vec2(xs, ys);

    b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);

    // handle keyboard input???
    switch(event.type) {
    case SDL_QUIT:
        m_closeGame = true;
        CC_CORE_INFO("SDL_QUIT Triggered.");
        break;

    case SDL_MOUSEBUTTONDOWN:
        if(event.button.button == SDL_BUTTON_LEFT) {
            mouse.MouseDown(pw);
        }
        else if(event.button.button == SDL_BUTTON_RIGHT) {
            mouse.RightMouseDown(pw);
        }
        break;

    case SDL_MOUSEBUTTONUP:
        if(event.button.button == SDL_BUTTON_LEFT) {
            mouse.MouseUp(pw);
        }
        else if(event.button.button == SDL_BUTTON_RIGHT) {
            mouse.RightMouseUp(pw);
        }
        break;

    case SDL_MOUSEMOTION:
        mouse.MouseMotion(ps);
        break;

    case SDL_MOUSEWHEEL:
        if(event.wheel.y > 0) {
            mouse.MouseWheelUp(pw);
        }
        else {
            mouse.MouseWheelDown(pw);
        }
        break;

    case SDL_KEYDOWN:
        switch(event.key.keysym.sym) {
        case SDLK_ESCAPE:
            m_closeGame = true;
            CC_CORE_INFO("ESC pressed!");
            CC_CORE_INFO("SDL_QUIT().");
            break;

        case SDLK_BACKQUOTE:
            CC_CORE_INFO("Calling game console!");

            break;
        case SDLK_LEFT:
            // Pan left
            // if(SDL_GetModState() == KMOD_LCTRL) {
            //     b2Vec2 newOrigin(2.0f, 0.0f);
            //     m_world->ShiftOrigin(newOrigin);
            // } how to use SDL_GetModState
            g_camera.m_center.x += 0.5f;

            break;

        case SDLK_RIGHT:
            // Pan right
            g_camera.m_center.x -= 0.5f;
            break;

        case SDLK_z:
            // Zoom out
            g_camera.m_zoom = b2Min(1.1f * g_camera.m_zoom, 20.0f);
            break;

        case SDLK_x:
            // Zoom in
            g_camera.m_zoom = b2Max(0.9f * g_camera.m_zoom, 0.02f);
            break;

        case SDLK_TAB:
            // show some tabs;
            break;
        default:
            not_handled = true;
            break;
        }
        break;
    }
}

/**
 * Show the debug draw if the g_settings.m_showDebugDraw is set to true.
 */
void GameContext::ShowDebugDraw() {
    if(g_settings.m_showDebugDraw) {
        m_world->DebugDraw();
    }
}

void GameContext::ShowHealthBar() {
    auto      view = m_reg.view< PhysicsComponent, HealthComponent >();  // TODO: temp sol, what if some entity dont have a b2Body ??
    QuadWrite writer(m_sdl_renderer.get());
    for(auto entity : view) {
        auto [body, health] = view.get< PhysicsComponent, HealthComponent >(entity);
        // TODO
        // traverse all fixture of a given body
        // for (b2Fixture* f = body.GetFixtureList(); f; f = f->GetNext())
        //     auto data = f->GetUserData();
        //     auto info = static_cast< EntityInfo >(data);
        // }
    }
}

Entity GameContext::CreateEntity() {
    return { this, m_reg.create() };
}

void GameContext::DestroyEntity(Entity entity) {
    m_reg.destroy(entity);
}
