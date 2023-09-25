#pragma once

#ifdef CC_PLATFORM_WINDOWS
#include <SDL.h>
#else 
#include <SDL2/SDL.h>
#endif

#include "Coconut/Log.h"
#include "Coconut/ECS/ECS.h"
#include "Coconut/Vector2D.h"
#include "Coconut/ConstantSpec.h"

namespace Coconut {

	class TransformComponent : public Coconut::Component {
	public:
		Vector2D position;
		Vector2D velocity;

		int speed = 3;

		int hight = Coconut::Constants::BIRD_HIGHT;
		int width = Coconut::Constants::BIRD_WIDTH;
		float scale = 1.0f;


		TransformComponent() : position(Vector2D()) {}
		TransformComponent(float x, float y) : position(Vector2D(x, y)) {};
		
		// Get the position of the entity
		std::tuple<float, float> getCoordinate() {
			return position.getCoordinate();
		}
	
		// Get the original size of the enity
		// @return (hight, width)
		std::tuple<int, int> getSize() {
			return std::tie(hight, width);
		}


		// ? casting warning ?s
		// Get the scaled size of the entity
		// @return (scaledHeight, scaledWidth)
		std::tuple<float, float> getScaledSize() {
			float scaledHeight = hight * scale;
			float scaledWidth = width * scale;
			return std::tie(scaledHeight, scaledWidth);
		}

		void init() override {
			velocity = Vector2D();

			CC_CORE_INFO("TransformComponent initialized!");
		}

		void update() override {
			//if (velocity.norm() > speed) {
			//	velocity /= velocity.norm();
			//	// this does not work! 
			//}
			position += velocity * speed;
		}

		void setPosition(float x, float y) {
			position = Vector2D(x, y);
		}
	};

}