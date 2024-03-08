#include "Core/GameContext.h"

#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Core/Log.h"

#include "Render/DebugDraw.h"

#include "Event/MouseEvent.h"
#include "util/sdl_check.h"
#include "util/sdl_delete.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"

extern Camera        g_camera;
static ImguiSettings s_imguiSettings;
Settings             g_settings;
DebugDraw            g_debugDraw;

GameContext::GameContext() {
    Init_SDL_Window();
    Init_SDL_Renderer();
    Init_Imgui();
    Init_Box2D();
    Init_DebugDraw();  // Notice: This should not init in release version

    LoadEntities();  // this should not be here?
    m_closeGame = false;
}

GameContext::~GameContext() {
    SDL_Quit();
}

// test function
void GameContext::LoadEntities() {

    // {
    //     auto box_entity = m_reg.create();

    //     auto boxSize = b2Vec2(1.0f, 1.0f);

    //     b2BodyDef bd;
    //     bd.type   = b2_dynamicBody;
    //     auto body = m_world->CreateBody(&bd);
    //     body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

    //     b2PolygonShape dynamicBox;
    //     dynamicBox.SetAsBox((boxSize.x / 2.0f) - dynamicBox.m_radius, (boxSize.y / 2.0f) - dynamicBox.m_radius);

    //     CC_CORE_INFO("box info:  hx: {}, hy: {}", (boxSize.x / 2.0f) - dynamicBox.m_radius, (boxSize.y / 2.0f) - dynamicBox.m_radius);

    //     b2FixtureDef fd;
    //     fd.shape       = &dynamicBox;
    //     fd.density     = 1;
    //     fd.friction    = 0.1f;
    //     fd.restitution = 0.5f;
    //     body->CreateFixture(&fd);

    //     m_reg.emplace< BodyComponent >(box_entity, body);
    //     m_reg.emplace< SpriteComponent >(box_entity, m_sdl_renderer.get(), "box.png");
    // }

    {
        Entity box_entity = CreateEntity();

        auto boxSize = b2Vec2(1.0f, 1.0f);

        b2BodyDef bd;
        bd.type   = b2_dynamicBody;
        auto body = m_world->CreateBody(&bd);
        body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));

        b2PolygonShape dynamicBox;
        dynamicBox.SetAsBox((boxSize.x / 2.0f) - dynamicBox.m_radius, (boxSize.y / 2.0f) - dynamicBox.m_radius);

        CC_CORE_INFO("box info:  hx: {}, hy: {}", (boxSize.x / 2.0f) - dynamicBox.m_radius, (boxSize.y / 2.0f) - dynamicBox.m_radius);

        b2FixtureDef fd;
        fd.shape       = &dynamicBox;
        fd.density     = 1;
        fd.friction    = 0.1f;
        fd.restitution = 0.5f;
        body->CreateFixture(&fd);

        box_entity.AddComponent< BodyComponent >(body);
        box_entity.AddComponent< SpriteComponent >(m_sdl_renderer.get(), "box.png");
    }

    {
        Entity edge = CreateEntity();
        
        b2BodyDef bd;
        bd.type = b2_staticBody;
        auto body = m_world->CreateBody(&bd);

        b2EdgeShape edgeShape;
        edgeShape.SetTwoSided(b2Vec2(-40.0f, -20.0f), b2Vec2(40.0f, -20.0f));
        body->CreateFixture(&edgeShape, 0.0f);

        edge.AddComponent< BodyComponent >(body);
    }
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

    m_sdl_window = SDL::Window(SDL_CreateWindow("SDL with box2d Game Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, g_camera.m_width, g_camera.m_height, SDL_WINDOW_SHOWN));

    if(m_sdl_window == nullptr) {
        CC_CORE_ERROR("SDL window failed to initialize! ");
        throw std::runtime_error("SDL_CreateWindow generate a NULL window");
    }

    CC_CORE_INFO("SDL window successfully initialized.");
}

void GameContext::Init_SDL_Renderer() {
    // m_SDL_Renderer = SDL_CreateRenderer(m_SDL_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    m_sdl_renderer = SDL::Renderer(SDL_CHECK(SDL_CreateRenderer(m_sdl_window.get(), -1, SDL_RENDERER_ACCELERATED)));

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

    m_textLine      = 30;
    m_textIncrement = 18;
    m_mouseJoint    = NULL;
    m_pointCount    = 0;
}

void GameContext::Init_DebugDraw() {
    g_debugDraw.Init(m_sdl_renderer.get());
    m_world->SetDebugDraw(&g_debugDraw);

    uint32 flags = 0;
    flags += g_settings.m_drawShapes * b2Draw::e_shapeBit;
    flags += g_settings.m_drawJoints * b2Draw::e_jointBit;
    flags += g_settings.m_drawAABBs * b2Draw::e_aabbBit;
    flags += g_settings.m_drawCOMs * b2Draw::e_centerOfMassBit;
    g_debugDraw.SetFlags(flags);
}

/**
 * Updates the UI for the game context.
 */
void GameContext::UpdateUI() {
    [[maybe_unused]] ImGuiIO& io = ImGui::GetIO();

    auto pw = b2Vec2(0.0f, 0.0f);
    auto ps = g_camera.ConvertWorldToScreen(pw);
    g_debugDraw.DrawString(5, m_textLine, "FPS: %.2f", ImGui::GetIO().Framerate);
    m_textLine += m_textIncrement;
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

        if(ImGui::Button("clear Entities")) {
            // m_entityManager->ClearEntities();
        }

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
        ImGui::End();
    }
}

void GameContext::Step() {
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

/**
 * Set the background color and render the background.
 *
 */
void GameContext::SetBackgroundColor() {
    ImGuiIO& io = ImGui::GetIO();
    SDL_RenderSetScale(m_sdl_renderer.get(), io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
    // set the bg color and render the background
    SDL_SetRenderDrawColor(m_sdl_renderer.get(), ( Uint8 )(m_clear_color.x * 255), ( Uint8 )(m_clear_color.y * 255), ( Uint8 )(m_clear_color.z * 255), ( Uint8 )(m_clear_color.w * 255));  // bg color
    SDL_RenderClear(m_sdl_renderer.get());
}

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
void GameContext::PollEvents() {
    SDL_Event event;
    while(SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);

        // handle mouse input
        int xs, ys;
        SDL_GetMouseState(&xs, &ys);
        auto ps = b2Vec2(xs, ys);

        b2Vec2 pw = g_camera.ConvertScreenToWorld(ps);

        // handle mouse and keyboard input
        CallHandleInput(event);
    }
}

void GameContext::RenderEntities() {
    auto      view = m_reg.view< BodyComponent, SpriteComponent >();  // TODO: temp sol, what if some entity dont have a b2Body ??
    QuadWrite writer(m_sdl_renderer.get());
    for(auto entity : view) {
        auto [body, sprite] = view.get< BodyComponent, SpriteComponent >(entity);
        auto box_size       = b2Vec2(1.0f, 1.0f);
        writer.UpdateRenderInfo(sprite.GetTexture(), box_size, body.GetPosition(), body.GetAngle());
        writer.Render();
    }
}

void GameContext::RemoveInactive() {
    auto view = m_reg.view< BodyComponent >();
    for(auto entity : view) {
        // TODO
    }
}

/**
 * Function called when a preset is submitted to SDL renderer.
 */
void GameContext::PresetSubmitted() {
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
void GameContext::RegisterInputCallback(InputCallback func) {
    func_InputCallback = func;
}

/**
 * CallHandleInput is a method of the GameContext class that handles input events.
 *
 * @param event the SDL_Event to be handled
 *
 * @return void
 *
 * @throws None
 */
void GameContext::CallHandleInput(SDL_Event& event) {
    if(func_InputCallback) {
        func_InputCallback(event);
    }
    else {
        DefaultInputCallback(event);
    }
}

// the default version of input callback
void GameContext::DefaultInputCallback(SDL_Event& event) {
    static MouseEvent mouse;
    while(SDL_PollEvent(&event)) {
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
                // TODO: test code
                LoadEntities();
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

// Another way to ....

// void GameContext::SetMouseEvent(MouseEvent& event) {
//     m_mouseEvent = event;
// }

// void GameContext::SetKeyboardEvent(KeyboardEvent& event) {
//     m_keyboardEvent = event;
// }

/**
 * Show the debug draw if the g_settings.m_showDebugDraw is set to true.
 */
void GameContext::ShowDebugDraw() {
    if(g_settings.m_showDebugDraw) {
        m_world->DebugDraw();
    }
}

Entity GameContext::CreateEntity() {
    return { this, m_reg.create() };
}

void GameContext::DestroyEntity(Entity entity) {
    m_reg.destroy(entity);
}