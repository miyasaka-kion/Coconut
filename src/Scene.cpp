#include "Scene.h"

#include <SDL2/SDL_rect.h>
#include <SDL_render.h>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "Settings.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include <SDL2/SDL.h>
#include <SDL_error.h>
#include <SDL_hints.h>
#include <thread> 

#include "Box.h"
#include "Constants.h"
#include "Edge.h"
#include "Entity.h"
#include "Log.h"
#include "Camera.h"
#include "Settings.h"
#include "DebugDraw.h"

extern Camera g_camera;
static ImguiSettings s_imguiSettings;
Settings g_settings;
DebugDraw g_debugDraw;

Scene::Scene() {
    // prepare game context
    Init_SDL_Window();
    Init_SDL_Renderer();
    Init_Imgui();
    Init_Box2D();
    if(g_settings.m_showDebugDraw) {
        Init_DebugDraw();
    }
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
    
    g_camera.m_width = g_settings.m_windowWidth;
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
    m_world = std::make_unique< b2World >(gravity);
    LoadEntities();
}
void Scene::UpdateUI() {
    [[maybe_unused]] ImGuiIO& io = ImGui::GetIO();
        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImVec2(float(g_camera.m_width), float(g_camera.m_height)));

    {
        if(s_imguiSettings.show_demo_window)
            ImGui::ShowDemoWindow();
    }

    {
        ImGui::Begin("Settings");
        ImGui::SliderFloat("Zoom Level", &g_camera.m_zoom, 0.0f, 1.0f);
        ImGui::Checkbox("show DebugDraw", &g_settings.m_showDebugDraw);
        ImGui::Checkbox("draw Sprites", &g_settings.m_drawSprites);
        ImGui::Checkbox("draw Shape", &g_settings.m_drawShapes);
        ImGui::Checkbox("draw Joint", &g_settings.m_drawJoints);
        ImGui::Checkbox("draw AABB", &g_settings.m_drawAABBs);
        ImGui::End();
    }

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static int entity_counter = 0;

        ImGui::Begin("Control bar");  // Create a window called "Hello, world!" and append into it.

        ImGui::Text("Adjust ...here!");                     // Display some text (you can use a format strings too)
        
        ImGui::Checkbox("Demo Window", &s_imguiSettings.show_demo_window);  // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &s_imguiSettings.show_another_window);

        auto gravity = m_world->GetGravity();
        ImGui::SliderFloat("gravity.y", &gravity.y, -10.0f, 0.0f);
        m_world->SetGravity(gravity);

        ImGui::ColorEdit3("clear color", ( float* )&m_clear_color);   // Edit 3 floats representing a color

        if(ImGui::Button("load box")) {
            LoadBox();
        }
        ImGui::SameLine();
        if(ImGui::Button("load Edge")) {
            LoadEdge();
        }
        ImGui::Text("counter = %d", entity_counter);

        if(ImGui::Button("clear Entities")) {
            m_entityList.clear();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void Scene::Run() {
    // game main loop

    bool  calculating = true;
    float progress    = 0.0f;

    while(m_closeGame != true) {
        [[maybe_unused]] auto io = ImGui::GetIO();
        PollEvents();
            

        UpdateUI();

        // render all
        ImGui::Render();
        SDL_RenderSetScale(m_SDL_Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_RenderClear(m_SDL_Renderer);

        // removeInactive(); this is currently unneeded!
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());
        

        RenderEntities();
        
        {
            // some SDL draw test 
            auto pw = b2Vec2(0.0f, 0.0f);
            auto ps = g_camera.ConvertWorldToScreen(pw);

            SDL_Rect rect{static_cast<int>(ps.x), static_cast<int>(ps.y), 10, 10};
            
            ImVec4 m_clear_color =  ImVec4(0.0f, 1.0f, 01.0f, 1.00f);
            SDL_SetRenderDrawColor(m_SDL_Renderer, ( Uint8 )(m_clear_color.x * 255), ( Uint8 )(m_clear_color.y * 255), ( Uint8 )(m_clear_color.z * 255), ( Uint8 )(m_clear_color.w * 255));

            // SDL_RenderDrawRect(m_SDL_Renderer, &rect);
            SDL_RenderDrawPoint(m_SDL_Renderer, ps.x, ps.y);
            
        }

        if(g_settings.m_showDebugDraw) {
            m_world->DebugDraw();
        }
          
        SDL_SetRenderDrawColor(m_SDL_Renderer, ( Uint8 )(m_clear_color.x * 255), ( Uint8 )(m_clear_color.y * 255), ( Uint8 )(m_clear_color.z * 255), ( Uint8 )(m_clear_color.w * 255));

        SDL_RenderPresent(m_SDL_Renderer);
        
        m_world->Step(1.0f / g_settings.m_hertz, g_settings.m_velocityIterations, g_settings.m_positionIterations);  // update

        m_world->ClearForces();
    }
}

void Scene::PollEvents() {
    while(SDL_PollEvent(&m_SDL_Event)) {
        ImGui_ImplSDL2_ProcessEvent(&m_SDL_Event);
        switch(m_SDL_Event.type) {
        case SDL_QUIT:
            m_closeGame = true;
            CC_CORE_INFO("SDL_QUIT Triggered.");
            break;
        case SDL_KEYDOWN:
            switch(m_SDL_Event.key.keysym.sym) {
            case SDLK_ESCAPE:
                m_closeGame = true;
                CC_CORE_INFO("ESC pressed!");
                CC_CORE_INFO("SDL_QUIT Triggered.");
                break;
            case SDLK_r:
                LoadBox();
                CC_CORE_INFO("r key pressed");
                break;

            case SDLK_a:
                g_camera.m_center.x -= 0.5f;
                CC_CORE_INFO("a key pressed");
                break;

            case SDLK_d:
                g_camera.m_center.x += 0.5f;
                CC_CORE_INFO("d key pressed");
                break;

            case SDLK_w:
                g_camera.m_center.y += 0.5f;
                CC_CORE_INFO("w key pressed");
                break;

            case SDLK_s:
                g_camera.m_center.y -= 0.5f;
                CC_CORE_INFO("s key pressed");
                break;
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

