#pragma once

#include <algorithm>
#include <string>

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

#include "Coconut/ECS/Compoent.h"

// This collider component is based on rectangle type of collision shape
namespace Coconut {
class ColliderComponent : public Coconut::Component {
public:
    SDL_Rect    collider;
    std::string tag;

    enum class Shape_t { rectangle = 0, circle };
    Shape_t shape;

    // use transform to determin the collider
    Coconut::TransformComponent* transform;

public:
    ColliderComponent(std::string&& str) : tag(std::move(str)) {}
    ColliderComponent(const std::string& str) : tag(str) {}

    // @return central, radius
    // where radius is the minimum of w and h

    float getCircCleRadius() const {
        return static_cast<float>(std::min(collider.w, collider.y));
    }

    Coconut::Vector2D getCircleCentral() const {
        float minRadius = getCircCleRadius();
        return Vector2D(static_cast<float>(collider.x + minRadius), static_cast<float>(collider.y + minRadius));
    }

    std::tuple<Coconut::Vector2D, float> getCircleInfo() const {
        float minRadius = getCircCleRadius();
        auto  central   = getCircleCentral();
        return std::tie(central, minRadius);
    }
 
    void init() override {
        this->shape = Shape_t::circle;
        if(entity->hasComponent<Coconut::TransformComponent>() == false) {
            entity->addComponent<Coconut::TransformComponent>();
        }
        transform = &entity->getComponent<Coconut::TransformComponent>();

        // cast exists here, maybe this is not a good manner, will be modifed in the future
        std::tie(collider.w, collider.h) = static_cast<std::tuple<float, float>>(transform->getScaledSize());
        std::tie(collider.x, collider.y) = transform->getCoordinate();
        CC_CORE_INFO("Collide box generated! tag = {}", tag);
        CC_CORE_INFO("the central of the box is {}, {}, radius is {}.", getCircleCentral().x, getCircleCentral().y, getCircCleRadius());
    }

    void update() override {
        std::tie(collider.x, collider.y) = transform->getCoordinate();
        if(this->shape == Shape_t::circle) {
        }
        else if(this->shape == Shape_t::rectangle) {
            // I don't want to implement this...
        }
    }
};
}  // namespace Coconut