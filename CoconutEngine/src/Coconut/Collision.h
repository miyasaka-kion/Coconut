#pragma once

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif 

#include "Coconut/ECS/TransformComponent.h"

namespace Coconut {
	class Collision {
	public:
		static bool rectangle(const SDL_Rect& a, const SDL_Rect& b);
		static bool circle(const SDL_Rect& a, const SDL_Rect& b);
		static bool circle(const Coconut::TransformComponent& ts_a, const Coconut::TransformComponent& ts_b);
	};
}
