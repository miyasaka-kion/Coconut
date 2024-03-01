#include "Scene.h"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_rect.h>
#include <SDL_keyboard.h>
#include <SDL_render.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_hints.h>
#include <thread>

#include "Camera.h"
#include "DebugDraw.h"
#include "Entity.h"
#include "Log.h"
#include "Settings.h"

// these will be removed in the future
#include "Constants.h"
#include "DebugObjects/Box.h"
#include "DebugObjects/Edge.h"


extern Camera        g_camera;
static ImguiSettings s_imguiSettings;
Settings             g_settings;
DebugDraw            g_debugDraw;

Scene::Scene() {
    // prepare game context
    Init_SDL_Window();
    Init_SDL_Renderer();
    Init_Imgui();
    Init_Box2D();
    Init_DebugDraw();  // TODO: This should not init in release version
    m_closeGame = false;
}

Scene::~Scene() {
    // clean up game context
    SDL_DestroyRenderer(m_SDL_Renderer);
    SDL_DestroyWindow(m_SDL_Window);
    SDL_Quit();
}

void Scene::Init_SDL_Renderer() {
    // m_SDL_Renderer = SDL_CreateRenderer(m_SDL_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    m_SDL_Renderer = SDL_CreateRenderer(m_SDL_Window, -1, SDL_RENDERER_ACCELERATED);
    if(m_SDL_Renderer == NULL) {
        CC_CORE_ERROR("SDL renderer initialization failed!");
        throw std::runtime_error("SDL_Renderer initialized a NULL renderer");
    }
    SDL_RendererInfo info;
    SDL_GetRendererInfo(m_SDL_Renderer, &info);
    CC_CORE_INFO("Current SDL_Renderer: {}", info.name);
}

void Scene::Init_SDL_Window() {
    // SDL_Init begin
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        CC_ERROR("SDL_Init error: {}", SDL_GetError());
    }

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width  = DM.w;
    auto Height = DM.h;

    CC_CORE_INFO("Width of the Screen: {}", Width);
    CC_CORE_INFO("Height of the Screen: {}", Height);

    CC_CORE_INFO("The rendering scale is {} pixels per meter. (px/1.0f)", c_pixelPerMeter);

    g_camera.m_width  = g_settings.m_windowWidth;
    g_camera.m_height = g_settings.m_windowHeight;

    m_SDL_Window = SDL_CreateWindow("SDL with box2d Game Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_camera.m_width, g_camera.m_height, SDL_WINDOW_SHOWN);

    if(m_SDL_Window == NULL) {
        CC_CORE_ERROR("SDL window failed to initialize! ");
        throw std::runtime_error("SDL_CreateWindow generate a NULL window");
    }
    CC_CORE_INFO("SDL window successfully initialized.");
}

void Scene::Init_Imgui() {
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
    ImGui_ImplSDL2_InitForSDLRenderer(m_SDL_Window, m_SDL_Renderer);
    ImGui_ImplSDLRenderer2_Init(m_SDL_Renderer);
}

void Scene::Init_DebugDraw() {
    g_debugDraw.Init(m_SDL_Renderer);
    m_world->SetDebugDraw(&g_debugDraw);

    uint32 flags = 0;
    flags += g_settings.m_drawShapes * b2Draw::e_shapeBit;
    flags += g_settings.m_drawJoints * b2Draw::e_jointBit;
    flags += g_settings.m_drawAABBs * b2Draw::e_aabbBit;
    flags += g_settings.m_drawCOMs * b2Draw::e_centerOfMassBit;
    g_debugDraw.SetFlags(flags);
}

void Scene::Init_Box2D() {
    // physics info initialize
    auto gravity = b2Vec2(0.0f, -10.0f);
    m_world      = std::make_unique< b2World >(gravity);

    m_textLine      = 30;
    m_textIncrement = 18;
    m_mouseJoint    = NULL;
    m_pointCount    = 0;
    LoadEntities();
}
void Scene::UpdateUI() {
    [[maybe_unused]] ImGuiIO& io = ImGui::GetIO();

    auto pw = b2Vec2(0.0f, 0.0f);
    auto ps = g_camera.ConvertWorldToScreen(pw);

    {
        // test demo windon
        if(s_imguiSettings.show_demo_window)
            ImGui::ShowDemoWindow();
    }

    {
        // box2d settings
        ImGui::Begin("Box2D Settings");
        ImGui::SliderFloat("Zoom Level", &g_camera.m_zoom, 0.0f, 1.0f);
        ImGui::Checkbox("show DebugDraw", &g_settings.m_showDebugDraw);
        ImGui::Checkbox("draw Sprites", &g_settings.m_drawSprites);

        auto gravity = m_world->GetGravity();
        ImGui::SliderFloat("gravity.y", &gravity.y, -10.0f, 0.0f);
        m_world->SetGravity(gravity);

        ImGui::SliderInt("Vel Iters", &g_settings.m_velocityIterations, 0, 50);
        ImGui::SliderInt("Pos Iters", &g_settings.m_positionIterations, 0, 50);
        ImGui::SliderFloat("Hertz", &g_settings.m_hertz, 5.0f, 120.0f, "%.0f hz");

        ImGui::Separator();

        ImGui::Checkbox("Sleep", &g_settings.m_enableSleep);
        ImGui::Checkbox("Warm Starting", &g_settings.m_enableWarmStarting);
        ImGui::Checkbox("Time of Impact", &g_settings.m_enableContinuous);
        ImGui::Checkbox("Sub-Stepping", &g_settings.m_enableSubStepping);

        ImGui::Separator();

        ImGui::Checkbox("Shapes", &g_settings.m_drawShapes);
        ImGui::Checkbox("Joints", &g_settings.m_drawJoints);
        ImGui::Checkbox("AABBs", &g_settings.m_drawAABBs);
        ImGui::Checkbox("Contact Points", &g_settings.m_drawContactPoints);
        ImGui::Checkbox("Contact Normals", &g_settings.m_drawContactNormals);
        ImGui::Checkbox("Contact Impulses", &g_settings.m_drawContactImpulse);
        ImGui::Checkbox("Friction Impulses", &g_settings.m_drawFrictionImpulse);
        ImGui::Checkbox("Center of Masses", &g_settings.m_drawCOMs);
        ImGui::Checkbox("Statistics", &g_settings.m_drawStats);
        ImGui::Checkbox("Profile", &g_settings.m_drawProfile);
        ImGui::End();
    }

    {
        ImGui::Begin("Window Settings");                                    // Create a window called "Hello, world!" and append into it.
        ImGui::Checkbox("Demo Window", &s_imguiSettings.show_demo_window);  // Edit bools storing our window open/close state
        ImGui::ColorEdit3("bg color", ( float* )&m_clear_color);            // Edit 3 floats representing a color

        if(ImGui::Button("load box")) {
            LoadBox();
        }
        ImGui::SameLine();
        if(ImGui::Button("load Edge")) {
            LoadEdge();
        }
        if(ImGui::Button("clear Entities")) {
            m_entityList.clear();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void Scene::Step() {
    float timeStep = g_settings.m_hertz > 0.0f ? 1.0f / g_settings.m_hertz : float(0.0f);

    if(g_settings.m_pause) {
        if(g_settings.m_singleStep) {
            g_settings.m_singleStep = 0;
        }
        else {
            timeStep = 0.0f;
        }

        g_debugDraw.DrawString(5, m_textLine, "****PAUSED****");
        m_textLine += m_textIncrement;
    }

    uint32 flags = 0;
    flags += g_settings.m_drawShapes * b2Draw::e_shapeBit;
    flags += g_settings.m_drawJoints * b2Draw::e_jointBit;
    flags += g_settings.m_drawAABBs * b2Draw::e_aabbBit;
    flags += g_settings.m_drawCOMs * b2Draw::e_centerOfMassBit;
    g_debugDraw.SetFlags(flags);

    m_world->SetAllowSleeping(g_settings.m_enableSleep);
    m_world->SetWarmStarting(g_settings.m_enableWarmStarting);
    m_world->SetContinuousPhysics(g_settings.m_enableContinuous);
    m_world->SetSubStepping(g_settings.m_enableSubStepping);

    m_pointCount = 0;

    m_world->Step(timeStep, g_settings.m_velocityIterations, g_settings.m_positionIterations);

    m_world->DebugDraw();

    if(timeStep > 0.0f) {
        ++m_stepCount;
    }

    if(g_settings.m_drawStats) {
        int32 bodyCount    = m_world->GetBodyCount();
        int32 contactCount = m_world->GetContactCount();
        int32 jointCount   = m_world->GetJointCount();
        g_debugDraw.DrawString(5, m_textLine, "bodies/contacts/joints = %d/%d/%d", bodyCount, contactCount, jointCount);
        m_textLine += m_textIncrement;

        int32 proxyCount = m_world->GetProxyCount();
        int32 height     = m_world->GetTreeHeight();
        int32 balance    = m_world->GetTreeBalance();
        float quality    = m_world->GetTreeQuality();
        g_debugDraw.DrawString(5, m_textLine, "proxies/height/balance/quality = %d/%d/%d/%g", proxyCount, height, balance, quality);
        m_textLine += m_textIncrement;
    }

    // Track maximum profile times
    {
        const b2Profile& p         = m_world->GetProfile();
        m_maxProfile.step          = b2Max(m_maxProfile.step, p.step);
        m_maxProfile.collide       = b2Max(m_maxProfile.collide, p.collide);
        m_maxProfile.solve         = b2Max(m_maxProfile.solve, p.solve);
        m_maxProfile.solveInit     = b2Max(m_maxProfile.solveInit, p.solveInit);
        m_maxProfile.solveVelocity = b2Max(m_maxProfile.solveVelocity, p.solveVelocity);
        m_maxProfile.solvePosition = b2Max(m_maxProfile.solvePosition, p.solvePosition);
        m_maxProfile.solveTOI      = b2Max(m_maxProfile.solveTOI, p.solveTOI);
        m_maxProfile.broadphase    = b2Max(m_maxProfile.broadphase, p.broadphase);

        m_totalProfile.step += p.step;
        m_totalProfile.collide += p.collide;
        m_totalProfile.solve += p.solve;
        m_totalProfile.solveInit += p.solveInit;
        m_totalProfile.solveVelocity += p.solveVelocity;
        m_totalProfile.solvePosition += p.solvePosition;
        m_totalProfile.solveTOI += p.solveTOI;
        m_totalProfile.broadphase += p.broadphase;
    }

    if(g_settings.m_drawProfile) {
        const b2Profile& p = m_world->GetProfile();

        b2Profile aveProfile;
        memset(&aveProfile, 0, sizeof(b2Profile));
        if(m_stepCount > 0) {
            float scale              = 1.0f / m_stepCount;
            aveProfile.step          = scale * m_totalProfile.step;
            aveProfile.collide       = scale * m_totalProfile.collide;
            aveProfile.solve         = scale * m_totalProfile.solve;
            aveProfile.solveInit     = scale * m_totalProfile.solveInit;
            aveProfile.solveVelocity = scale * m_totalProfile.solveVelocity;
            aveProfile.solvePosition = scale * m_totalProfile.solvePosition;
            aveProfile.solveTOI      = scale * m_totalProfile.solveTOI;
            aveProfile.broadphase    = scale * m_totalProfile.broadphase;
        }

        g_debugDraw.DrawString(5, m_textLine, "step [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.step, aveProfile.step, m_maxProfile.step);
        m_textLine += m_textIncrement;
        g_debugDraw.DrawString(5, m_textLine, "collide [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.collide, aveProfile.collide, m_maxProfile.collide);
        m_textLine += m_textIncrement;
        g_debugDraw.DrawString(5, m_textLine, "solve [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solve, aveProfile.solve, m_maxProfile.solve);
        m_textLine += m_textIncrement;
        g_debugDraw.DrawString(5, m_textLine, "solve init [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveInit, aveProfile.solveInit, m_maxProfile.solveInit);
        m_textLine += m_textIncrement;
        g_debugDraw.DrawString(5, m_textLine, "solve velocity [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveVelocity, aveProfile.solveVelocity, m_maxProfile.solveVelocity);
        m_textLine += m_textIncrement;
        g_debugDraw.DrawString(5, m_textLine, "solve position [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solvePosition, aveProfile.solvePosition, m_maxProfile.solvePosition);
        m_textLine += m_textIncrement;
        g_debugDraw.DrawString(5, m_textLine, "solveTOI [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.solveTOI, aveProfile.solveTOI, m_maxProfile.solveTOI);
        m_textLine += m_textIncrement;
        g_debugDraw.DrawString(5, m_textLine, "broad-phase [ave] (max) = %5.2f [%6.2f] (%6.2f)", p.broadphase, aveProfile.broadphase, m_maxProfile.broadphase);
        m_textLine += m_textIncrement;
    }

    if(g_settings.m_drawContactPoints) {
        const float k_impulseScale = 0.1f;
        const float k_axisScale    = 0.3f;

        for(int32 i = 0; i < m_pointCount; ++i) {
            ContactPoint* point = m_points + i;

            if(point->state == b2_addState) {
                // Add
                g_debugDraw.DrawPoint(point->position, 10.0f, b2Color(0.3f, 0.95f, 0.3f));
            }
            else if(point->state == b2_persistState) {
                // Persist
                g_debugDraw.DrawPoint(point->position, 5.0f, b2Color(0.3f, 0.3f, 0.95f));
            }

            if(g_settings.m_drawContactNormals == 1) {
                b2Vec2 p1 = point->position;
                b2Vec2 p2 = p1 + k_axisScale * point->normal;
                g_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.9f));
            }
            else if(g_settings.m_drawContactImpulse == 1) {
                b2Vec2 p1 = point->position;
                b2Vec2 p2 = p1 + k_impulseScale * point->normalImpulse * point->normal;
                g_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
            }

            if(g_settings.m_drawFrictionImpulse == 1) {
                b2Vec2 tangent = b2Cross(point->normal, 1.0f);
                b2Vec2 p1      = point->position;
                b2Vec2 p2      = p1 + k_impulseScale * point->tangentImpulse * tangent;
                g_debugDraw.DrawSegment(p1, p2, b2Color(0.9f, 0.9f, 0.3f));
            }
        }
    }
}

void Scene::Run() {
    // game main loop

    bool  calculating = true;
    float progress    = 0.0f;

    while(m_closeGame != true) {
        [[maybe_unused]] auto io = ImGui::GetIO();
        // ImGui context begin
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        // debugDraw m_textLine reset
        m_textLine = 26;

        PollEvents();

        UpdateUI();

        g_debugDraw.DrawString(5, m_textLine, "FPS: %.2f", ImGui::GetIO().Framerate);
        m_textLine += m_textIncrement;

        SDL_RenderSetScale(m_SDL_Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

        // set the bg color and render the background
        SDL_SetRenderDrawColor(m_SDL_Renderer, ( Uint8 )(m_clear_color.x * 255), ( Uint8 )(m_clear_color.y * 255), ( Uint8 )(m_clear_color.z * 255), ( Uint8 )(m_clear_color.w * 255));  // bg color
        SDL_RenderClear(m_SDL_Renderer);

        RenderEntities();

        {
            // some SDL draw test
            auto pw = b2Vec2(0.0f, 0.0f);
            auto ps = g_camera.ConvertWorldToScreen(pw);

            SDL_Rect rect{ static_cast< int >(ps.x), static_cast< int >(ps.y), 10, 10 };

            ImVec4 m_clear_color = ImVec4(0.0f, 1.0f, 01.0f, 1.00f);
            SDL_SetRenderDrawColor(m_SDL_Renderer, ( Uint8 )(m_clear_color.x * 255), ( Uint8 )(m_clear_color.y * 255), ( Uint8 )(m_clear_color.z * 255), ( Uint8 )(m_clear_color.w * 255));

            // SDL_RenderDrawRect(m_SDL_Renderer, &rect);
            SDL_RenderDrawPoint(m_SDL_Renderer, ps.x, ps.y);
        }

        if(g_settings.m_showDebugDraw) {
            m_world->DebugDraw();
        }

        Step();
        ImGui::Render();
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        // ImGui context end
        SDL_RenderPresent(m_SDL_Renderer);
        m_world->ClearForces();
    }
}

void Scene::PollEvents() {
    while(SDL_PollEvent(&m_SDL_Event)) {
        ImGui_ImplSDL2_ProcessEvent(&m_SDL_Event);

        // handle mouse input
        int xs, ys;
        SDL_GetMouseState(&xs, &ys);
        auto ps = b2Vec2(xs, ys);

        b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);

        // handle keyboard input???
        switch(m_SDL_Event.type) {
        case SDL_QUIT:
            m_closeGame = true;
            CC_CORE_INFO("SDL_QUIT Triggered.");
            break;

        case SDL_MOUSEBUTTONDOWN:
            if(m_SDL_Event.button.button == SDL_BUTTON_LEFT) {
                m_mouse.MouseDown(pw);
            }
            else if(m_SDL_Event.button.button == SDL_BUTTON_RIGHT) {
                m_mouse.RightMouseDown(pw);
            }
            break;

        case SDL_MOUSEBUTTONUP:
            if(m_SDL_Event.button.button == SDL_BUTTON_LEFT) {
                m_mouse.MouseUp(pw);
            }
            else if(m_SDL_Event.button.button == SDL_BUTTON_RIGHT) {
                m_mouse.RightMouseUp(pw);
            }
            break;

        case SDL_MOUSEMOTION:
            m_mouse.MouseMotion(ps);
            break;

        case SDL_MOUSEWHEEL:
            if(m_SDL_Event.wheel.y > 0) {
                m_mouse.MouseWheelUp(pw);
            }
            else {
                m_mouse.MouseWheelDown(pw);
            }
            break;

        case SDL_KEYDOWN:
            switch(m_SDL_Event.key.keysym.sym) {
            case SDLK_ESCAPE:
                m_closeGame = true;
                CC_CORE_INFO("ESC pressed!");
                CC_CORE_INFO("SDL_QUIT Triggered.");
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
                g_camera.m_center.x -= 0.5f;

                break;

            case SDLK_RIGHT:
                // Pan right
                g_camera.m_center.x += 0.5f;
                break;

            case SDLK_r:
                LoadBox();  // TODO: remove this
                CC_CORE_INFO("key [r] pressed");
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

                // control player move by default.
                // case SDLK_a:
                //     g_camera.m_center.x -= 0.5f;
                //     CC_CORE_INFO("a key pressed");
                //     break;

                // case SDLK_d:
                //     g_camera.m_center.x += 0.5f;
                //     CC_CORE_INFO("d key pressed");
                //     break;

                // case SDLK_w:
                //     g_camera.m_center.y += 0.5f;
                //     CC_CORE_INFO("w key pressed");
                //     break;

                // case SDLK_s:
                //     g_camera.m_center.y -= 0.5f;
                //     CC_CORE_INFO("s key pressed");
                //     break;
            }
            break;
        }
    }
}

void Scene::RenderEntities() {
    if(g_settings.m_drawSprites) {
        for(const auto& entity : m_entityList) {
            entity->Render();
        }
    }
}

void Scene::RemoveInactive() {
    m_entityList.erase(std::remove_if(std::begin(m_entityList), std::end(m_entityList), [](const std::unique_ptr< Entity >& entity) { return !entity->isActive; }), m_entityList.end());
}

// test part
void Scene::LoadEntities() {
    LoadBox();
    LoadEdge();
}

void Scene::LoadBox() {
    auto box = std::make_unique< Box >(m_world.get(), m_SDL_Renderer);

    box->Init(c_OriginPos, b2Vec2(c_OriginalBoxWidth, c_OriginalBoxHeight), c_OriginalVelocity, c_originalAngle);

    m_entityList.push_back(std::move(box));
}

void Scene::LoadEdge() {
    // some constants
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -30.0f;
    startpoint.y = -20.0f;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 30.0f;
    endpoint.y = -20.0f;
    // constants end

    auto edge = std::make_unique< Edge >(m_world.get(), m_SDL_Renderer);
    edge->Init(startpoint, endpoint);

    m_entityList.push_back(std::move(edge));
}
