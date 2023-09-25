#include "Coconut/Collision.h"
#include "Coconut/ConstantSpec.h"
#include "Coconut/ECS/ColliderComponent.h"
#include "Coconut/Vector2D.h"
#include <iterator>

bool Coconut::Collision::rectangle (const SDL_Rect& a, const SDL_Rect& b) {
    // ToDo
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
    std::tie<Vector2D, float>(centralA, radiusA) = collider_a.getCircleINfo();
    std::tie<Vector2D, float>(centralB, radiusB) = collider_b.getCircleINfo();
    if(Coconut::Vector2D::distance(centralA, centralB) <= radiusA + radiusB) {
        return true;
    }
    
    return false;
}

