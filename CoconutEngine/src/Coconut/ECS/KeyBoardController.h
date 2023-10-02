#pragma once

#include "SDL.h"

#include "Coconut/ECS/ECS.h"
#include "Coconut/ECS/TransformComponent.h"
#include "Coconut/Game.h"
#include "Coconut/Log.h"

namespace Coconut {
class KeyboardController : public Coconut::Component {
public:
    TransformComponent* transform;

    void init() override {
        transform = &entity->getComponent<TransformComponent>();
        CC_CORE_INFO("KeyboardController initialized!");
    }

    void update() override {
        if(Coconut::Game::event.type == SDL_KEYDOWN) {
            CC_CORE_INFO("key pressed!");
            switch(Coconut::Game::event.key.keysym.sym) {
            case SDLK_w:
                transform->velocity.setY(-1);
                break;
            case SDLK_s:
                transform->velocity.setY(1);
                break;
            case SDLK_a:
                transform->velocity.setX(-1);
                break;
            case SDLK_d:
                transform->velocity.setX(1);
                break;
            default:
                break;
            }
        }

        if(Coconut::Game::event.type == SDL_KEYUP) {
            CC_CORE_INFO("keys up!");

            switch(Coconut::Game::event.key.keysym.sym) {
            case SDLK_w:
                transform->velocity.setY(0);
                break;
            case SDLK_s:
                transform->velocity.setY(0);
                break;
            case SDLK_a:
                transform->velocity.setX(0);
                break;
            case SDLK_d:
                transform->velocity.setX(0);
                break;
            default:
                break;
            }
        }
    }
};
}  // namespace Coconut