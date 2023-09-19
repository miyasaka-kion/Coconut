#pragma once

#include <SDL.h>

#include "Coconut/Log.h"
#include "Coconut/ECS/ECS.h"
#include "Coconut/Vector2D.h"


namespace Coconut {

	class TransformComponent : public Coconut::Component {
	public:
		Vector2D position;
		Vector2D velocity;

		int speed = 3;



		TransformComponent() : position(Vector2D()) {}
		TransformComponent(float x, float y) : position(Vector2D(x, y)) {};
		
		std::tuple<float, float> getCoordinate() {
			return position.getCoordinate();
		}

		

		void init() override {
			velocity = Vector2D();
			CC_CORE_INFO("TransformComponent initialized!");
		}

		void update() override {
			position += velocity * speed;
		}

		void setPosition(float x, float y) {
			position = Vector2D(x, y);
		}
	};

}