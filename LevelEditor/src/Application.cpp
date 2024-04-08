#include "Application.h"

#include <SDL2/SDL_keycode.h>

#include "Core/GameContext.h"
#include "Core/Log.h"
#include "Core/PhysicsInfo.h"
#include "Core/TextureManager.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include "Render/SpriteLoader.h"
#include "Util/imgui_include.h"

#include "Player.h"

Application::Application() {
    m_game = std::make_unique< GameContext >();
    // m_renderer = m_game->GetRenderer();
    m_game->RegisterClientHandleEvent([this](SDL_Event& event) -> bool { return ClientHandleEvent(event); });
    // Regester input callback here, if needed. If not registered the default input callback will be used.

    m_game->AddImGuiLayer< PlayerSettingsPanel >(m_playerSettings);
}

void Application::LoadEntities() {
    {
        // Entity box_entity = m_game->CreateEntity();
        auto boxSize = b2Vec2(1.0f, 1.0f);  // TODO Store this ...

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

        auto f = body->CreateFixture(&fd);

        auto box = m_game->CreateEntity();

        const auto& info = m_game->GetSpriteInfo("TEST_BOX_0");

        box.AddComponent< SpriteComponent >(info);
        box.AddComponent< PhysicsComponent >(body);
        box.AddComponent< PlayerComponent >();

        // for(int i = 0; i <= 29; i++) {
        //     auto        body                  = m_game->m_world->CreateBody(&bd);
        //     std::string name                  = "INGAME_BLOCKS_WOOD_1_" + std::to_string(i);
        //     auto        f                     = body->CreateFixture(&fd);
        //     auto        box_entity_on_fixture = m_game->CreateEntity();
        //     const auto& info                  = m_game->GetSpriteInfo(name);
        //     box_entity_on_fixture.AddComponent< SpriteComponent >(info);
        //     box_entity_on_fixture.AddComponent< PhysicsComponent >(body);
        // }

        // box_fixture.AddComponent< BodyComponent >(body);
    }

    {
        Entity edge = m_game->CreateEntity();

        b2BodyDef bd;
        bd.type   = b2_staticBody;
        auto body = m_game->m_world->CreateBody(&bd);

        b2EdgeShape edgeShape;
        edgeShape.SetTwoSided(b2Vec2(-40.0f, -20.0f), b2Vec2(40.0f, -20.0f));
        auto f = body->CreateFixture(&edgeShape, 0.0f);

        f->SetFriction(1.0f);
    }
}

/**
 * The Run function runs the application and contains the main game loop.
 */
void Application::Run() {
    CC_INFO("Application Running!");

    // TODO: Serialization process
    LoadEntities();

    // game main loop
    while(m_game->isClosed() != true) {
        // [[maybe_unused]] auto io = ImGui::GetIO();
        // ImGui context begin
        m_game->NewFrame();

        m_game->HandleEvent();

        // ImGui context end

        m_game->UpdateUI();

        m_game->SetBackground();  // hmm.. this should be done in NewFrame?

        // add Logic here ...

        m_game->RenderEntities();

        m_game->ShowDebugDraw();

        m_game->Step();

        m_game->PresentSubmitted();

        m_game->GetWorld()->ClearForces();  // clear forces after stepping
    }
}

bool Application::HandlePlayerInput(SDL_Keycode sym) {
    switch(sym) {
    case SDLK_r: {
        LoadEntities();
        break;
    }
    // control player move by default.
    case SDLK_SPACE:
        [[fallthrough]];

    case SDLK_w: {
        // player jump

        break;
    }

    case SDLK_d: {

        auto view = m_game->GetRegistry().view< PlayerComponent, PhysicsComponent >();
        for(auto [entity, player, physics] : view.each()) {
            auto body = physics.GetBody();
            body->ApplyForceToCenter(b2Vec2(m_playerSettings.move_force, 0.0f), true);
        }
        break;
    }

    case SDLK_a: {
        // player move left
        auto view = m_game->GetRegistry().view< PlayerComponent, PhysicsComponent >();
        for(auto [entity, player, physics] : view.each()) {
            auto body = physics.GetBody();
            body->ApplyForceToCenter(b2Vec2(-m_playerSettings.move_force, 0.0f), true);
        }
        break;
    }

    case SDLK_s: {
        // player crouch

        break;
    }

    case SDLK_f: {
        // player shoot

        break;
    }

    default: {
        return false;
    }
    }  // switch
    return true;
}

bool Application::ClientHandleEvent(SDL_Event& event) {
    switch(event.type) {

    case SDL_KEYDOWN: {
        auto sym = event.key.keysym.sym;
        if(HandlePlayerInput(sym)) {
            break;
        }
    }
    default: {
        return false;
    }
    }
    return true;
}