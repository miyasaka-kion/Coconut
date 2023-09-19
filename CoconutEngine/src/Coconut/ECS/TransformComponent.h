#pragma once

#include <Coconut/ECS/ECS.h>
#include <Coconut/ECS/Compoent.h>
#include <Coconut/Vector2D.h>


namespace Coconut {

	class TransformComponent : public Coconut::Component {
	public:
		Vector2D position;

		TransformComponent() : position(Vector2D()) {}
		TransformComponent(float x, float y) : position(Vector2D(x, y)) {};
		
		std::tuple<float, float> getCoordinate() {
			return position.getCoordinate();
		}

		

		void init() override {
			
		}

		void update() override {
			position += Vector2D(1, 1);
		}

		void setPosition(float x, float y) {
			position = Vector2D(x, y);
		}
	};

}