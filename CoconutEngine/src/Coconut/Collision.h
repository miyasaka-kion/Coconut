#pragma once

#include "Coconut/ECS/ColliderComponent.h"
#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif 

#include "Coconut/ECS/TransformComponent.h"

namespace Coconut {
	class Collision {
	public:
		// this will be the collide judge for any 2 type of TransformComponent
		static bool isCollide(const Coconut::ColliderComponent& collider_a, const Coconut::ColliderComponent& collider_b);
	// this should be set to private?
	public:
		static bool rectangle(const SDL_Rect& a, const SDL_Rect& b);
		static bool rectangle(const Coconut::ColliderComponent& collider_a, const Coconut::ColliderComponent& collider_b);

		static bool circle(const SDL_Rect& a, const SDL_Rect& b);
		static bool circle(const Coconut::ColliderComponent& collider_a, const Coconut::ColliderComponent& collider_b);
		static bool circle(const Coconut::Entity& entity_a, const Coconut::Entity& entity_b);
	};
}
