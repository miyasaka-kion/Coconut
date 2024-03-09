#include "Application.h"

#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

#include "Core/GameContext.h"
#include "Core/Log.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"

Application::Application() {
    m_game     = std::make_unique< GameContext >();
    m_renderer = m_game->GetRenderer();
    m_game->RegisterClientHandleEvent([this](SDL_Event& event) -> bool {
        return ClientHandleEvent(event);
    });
    // Regester input callback here, if needed. If not registered the default input callback will be used.
}

void Application::LoadEntities() {
    {
        Entity box_entity = m_game->CreateEntity();

        auto boxSize = b2Vec2(1.0f, 1.0f);

        b2BodyDef bd;
        bd.type   = b2_dynamicBody;
        auto body = m_game->m_world->CreateBody(&bd);
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
        box_entity.AddComponent< SpriteComponent >(m_renderer, "box.png");
    }

    {
        Entity edge = m_game->CreateEntity();

        b2BodyDef bd;
        bd.type   = b2_staticBody;
        auto body = m_game->m_world->CreateBody(&bd);

        b2EdgeShape edgeShape;
        edgeShape.SetTwoSided(b2Vec2(-40.0f, -20.0f), b2Vec2(40.0f, -20.0f));
        body->CreateFixture(&edgeShape, 0.0f);

        edge.AddComponent< BodyComponent >(body);
    }
}

/**
 * The Run function runs the application and contains the main game loop.
 */
void Application::Run() {
    CC_INFO("Application Running!");
    LoadEntities();

    // game main loop
    while(m_game->isClosed() != true) {
        // [[maybe_unused]] auto io = ImGui::GetIO();
        // ImGui context begin
        m_game->NewFrame();

        m_game->PollAndHandleEvents();

        // ImGui context end

        m_game->UpdateUI();

        m_game->SetBackgroundColor();  // hmm.. this should be done in NewFrame?

        // add Logic here ...

        m_game->RenderEntities();

        m_game->ShowDebugDraw();

        m_game->Step();

        m_game->PresetSubmitted();

        m_game->GetWorld()->ClearForces();  // clear forces after stepping
    }
}

bool Application::ClientHandleEvent(SDL_Event& event) {
    switch(event.type) {

    case SDL_KEYDOWN: {
        auto sym = event.key.keysym.sym;
        switch(sym) {
        case SDLK_r:
            LoadEntities();
            return true;
        // control player move by default.
        case SDLK_a:
            [[fallthrough]];
        case SDLK_d:
            [[fallthrough]];
        case SDLK_w:
            [[fallthrough]];
        case SDLK_s:
            g_camera.m_center.x += ((sym == SDLK_a) - (sym == SDLK_d)) * 0.5f;
            g_camera.m_center.y -= ((sym == SDLK_w) - (sym == SDLK_s)) * 0.5f;
            return true;
        }
    } break; 
    } // switch  

    return false;
}