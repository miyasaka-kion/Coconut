#include <iterator>

#include "Coconut/Collision.h"
#include "Coconut/ConstantSpec.h"
#include "Coconut/ECS/ColliderComponent.h"
#include "Coconut/Vector2D.h"
#include "Coconut/ECS/TransformComponent.h"


bool Coconut::Collision::rectangle (const SDL_Rect& a, const SDL_Rect& b) {
    // ToDo

    return false;
}

bool Coconut::Collision::rectangle(const Coconut::ColliderComponent& collider_a, const Coconut::ColliderComponent& collider_b)
{
    Coconut::TransformComponent transA = *(collider_a.transform);
    Coconut::TransformComponent transB = *(collider_b.transform);
    
    auto relativePos = transA.getRelativePosition(transB);
    

    // We fix Point A and get the relative position of B subject to A 
    if (relativePos == TransformComponent::RelativePosition_t::lu) {
        // Point at the opposite diagonal position
        Coconut::Vector2D rdB = transB.getPositionVector2D() + transB.getSizeVector2D();
        if (rdB > transA.getPositionVector2D()) return true;
        else return false;
    }
    //if (relativePos == TransformComponent::RelativePosition_t::ru) {
    //    Coconut::Vector2D ldB = transB.getPositionVector2D() + Vector2D(0.0f, static_cast<float> (transB.getHight()));
    //    if (ldB.getX() > 
    //}
    // Shit I dont wanna impl this !!!!
    return false;
}

bool Coconut::Collision::circle(const SDL_Rect& a, const SDL_Rect& b) {
    // ToDo
    return false;
}


// Collision with other circle
bool Coconut::Collision::circle(const Coconut::ColliderComponent& collider_a, const Coconut::ColliderComponent& collider_b) { 
    // ToDo
    Coconut::Vector2D centralA;
    float radiusA;
    Coconut::Vector2D centralB;
    float radiusB;
    std::tie<Vector2D, float>(centralA, radiusA) = collider_a.getCircleInfo();
    std::tie<Vector2D, float>(centralB, radiusB) = collider_b.getCircleInfo();
    if(Coconut::Vector2D::distance(centralA, centralB) <= radiusA + radiusB) {
        return true;
    }
    
    return false;
}

bool Coconut::Collision::circle(const Coconut::Entity& entity_a, const Coconut::Entity& entity_b)
{
    auto& collider_a = entity_a.getComponent<ColliderComponent>();
    auto& collider_b = entity_b.getComponent<ColliderComponent>();

    return circle(collider_a, collider_b);
}

