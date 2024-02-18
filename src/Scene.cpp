#include "Scene.h"

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

#include "Box.h"
#include "Constants.h"
#include "Edge.h"
#include "Entity.h"
#include "Log.h"
#include "MetricConverter.h"
#include "Settings.h"
#include "Camera.h"

Camera g_camera;
static Settings s_settings;

Scene::Scene() {
    // prepare game context
    init_sdl_window();
    init_sdl_renderer();
    init_imgui();

    // physics info initialize
    auto gravity = b2Vec2(0.0f, -10.0f);
    world = std::make_unique< b2World >(gravity);
    loadEntities();
    closeGame = false;
}

Scene::~Scene() {
    // clean up game context
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Scene::init_sdl_renderer() {
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        CC_CORE_ERROR("SDL renderer initialization failed!");
        throw std::runtime_error("SDL_Renderer initialized a NULL renderer");
    }
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    CC_CORE_INFO("Current SDL_Renderer: {}", info.name);
}

void Scene::run() {
    // game main loop
    [[maybe_unused]] ImGuiIO& io = ImGui::GetIO();

    bool calculating = true;
    float progress = 0.0f;

    bool show_demo_window    = true;
    bool show_another_window = false;

    while(closeGame != true) {
        pollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        auto gravity = world->GetGravity();
    
        {
            if(show_demo_window)
                ImGui::ShowDemoWindow();
        }

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            static int entity_counter = 0;

            ImGui::Begin("Control bar");  // Create a window called "Hello, world!" and append into it.

            ImGui::Text("Adjust ...here!");           // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);  // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("gravity.y", &gravity.y, -10.0f, 0.0f);               // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", ( float* )&clear_color);  // Edit 3 floats representing a color

            

            if(ImGui::Button("load box")) {
                loadBox();            
            }
            ImGui::SameLine();
            if(ImGui::Button("load Edge")) {
                loadEdge();
            }
            ImGui::Text("counter = %d", entity_counter);
            if(ImGui::Button("clear Entities")) {
                entityList.clear();
            }
    
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }
        

        // update world info 
        world->SetGravity(gravity);

        // update world info end

        // render all
        ImGui::Render();
        SDL_RenderSetScale(renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);

        SDL_RenderClear(renderer);

        // removeInactive(); this is currently unneeded!
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData());

        refresh();
        SDL_SetRenderDrawColor(renderer, ( Uint8 )(clear_color.x * 255), ( Uint8 )(clear_color.y * 255), ( Uint8 )(clear_color.z * 255), ( Uint8 )(clear_color.w * 255));

        SDL_RenderPresent(renderer);

        world->Step(1.0f / s_settings.m_hertz, s_settings.m_velocityIterations, s_settings.m_positionIterations);  // update

        world->ClearForces();
    }
}

void Scene::init_sdl_window() {
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
    
    g_camera.m_width = s_settings.m_windowWidth;
	g_camera.m_height = s_settings.m_windowHeight;

    window = SDL_CreateWindow("SDL with box2d Game Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_camera.m_width, g_camera.m_height, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        CC_CORE_ERROR("SDL window failed to initialize! ");
        throw std::runtime_error("SDL_CreateWindow generate a NULL window");
    }
    CC_CORE_INFO("SDL window successfully initialized.");
}

void Scene::init_imgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ( void )io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable; // Enable docking functionalities
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);
}

void Scene::pollEvents() {
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if(event.type == SDL_QUIT) {
            closeGame = true;
            CC_CORE_INFO("SDL_QUIT Triggered.");
        }

        else if(event.key.keysym.sym == SDLK_ESCAPE) {
            closeGame = true;
            CC_CORE_INFO("ESC pressed!");
            CC_CORE_INFO("SDL_QUIT Triggered.");
        }

        else if(event.key.keysym.sym == SDLK_r) {
            loadBox();
            CC_CORE_INFO("r key pressed");
        }
    }
}

void Scene::refresh() {
    for(const auto& entity : entityList) {
        entity->Render();
    }
}

void Scene::removeInactive() {
    entityList.erase(std::remove_if(std::begin(entityList), std::end(entityList), [](const std::unique_ptr< Entity >& entity) { return !entity->isActive; }), entityList.end());
}

// test part
void Scene::loadEntities() {
    loadBox();
    loadEdge();
}

void Scene::loadBox() {
    auto box = std::make_unique< Box >(world.get(), renderer);

    box->Init(c_OriginPos, b2Vec2(c_OriginalBoxWidth, c_OriginalBoxHeight), c_OriginalVelocity, c_originalAngle);
    
    entityList.push_back(std::move(box));
}

void Scene::loadEdge() {
    // some constants
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -3.0f;
    startpoint.y = -2.0;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3.0;
    endpoint.y = -2.0;
    // constants end

    auto edge = std::make_unique< Edge >(world.get(), renderer);
    edge->Init(startpoint, endpoint);

    entityList.push_back(std::move(edge));
}

