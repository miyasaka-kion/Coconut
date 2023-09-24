#pragma once

#include <string>
#include <algorithm>

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif 

#include "Coconut/ECS/Compoent.h"

namespace Coconut {
	class ColliderComponent : public Coconut::Component {
	public:
		SDL_Rect collider;
	
		std::string tag;

		enum class Shape_t {
			rectangle = 0,
			circle
		};
		Shape_t shape;

		Coconut::TransformComponent* transform;

		// @return central, radius
		std::tuple<Coconut::Vector2D, float> getCircleINfo() {
			float minRadius = static_cast<float>(std::min(collider.w, collider.y));
			Coconut::Vector2D central(static_cast<float>(collider.x + minRadius), static_cast<float>(collider.y + minRadius));
			return std::tie(central, minRadius);
		}

		void init() override {
			this->shape = Shape_t::circle;
			if (entity->hasComponent<Coconut::TransformComponent>() == false) {
				entity->addComponent<Coconut::TransformComponent>();
			}
			transform = &entity->getComponent<Coconut::TransformComponent>();
		}

		void update() override {
			std::tie(collider.x, collider.y) = transform->getCoordinate();
			if (this->shape == Shape_t::circle) {
				
			}
			else if (this->shape == Shape_t::rectangle) {
				// I don't want to implement this...
			}
		}

	};
}