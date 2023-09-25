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

	public:
		int speed = 3;
		int width = 32;
		int height = 32;
		float scale = Coconut::Constants::defaultScale;

	public:
		TransformComponent() : position(Vector2D()) {}
		TransformComponent(float x, float y) : position(Vector2D(x, y)) {};
		TransformComponent(float x, float y, int speed_p, int width_p, int height_p, float scale_p) {
			position = Vector2D(x, y);
			velocity = Vector2D();
			speed = speed_p;
			width = width_p;
			height = height_p;
			scale = scale_p;
		}

		enum class RelativePosition_t {
			same = 0,
			// left up
			lu,
			// left down
			ld,
			// right up
			ru,
			// right down
			rd
		};

		RelativePosition_t getRelativePosition(const TransformComponent& other) {
			return static_cast<TransformComponent::RelativePosition_t> (this->position.getRelativePosition(other.position));
		}
		
		float getPosX() {
			return position.x;
		}

		float getPosY() {
			return position.y;
		}

		int getHight() {
			return height;
		}

		int getWidth() {
			return width;
		}

		
		// Get the position of the entity
		std::tuple<float, float> getCoordinate() {
			return position.getCoordinate();
		}
	
		// Get the original size of the enity
		// @return (hight, width)
		std::tuple<int, int> getSize() {
			return std::tie(height, width);
		}

		Coconut::Vector2D getSizeVector2D() {
			return Vector2D(this->getSize());
		}

		Coconut::Vector2D getPositionVector2D() {
			return position;
		}

		// ? casting warning ?s
		// Get the scaled size of the entity
		// @return (scaledWidth, scaledHeight)
		std::tuple<float, float> getScaledSize() {
			float scaledHeight = height * scale;
			float scaledWidth = width * scale;
			return std::tie(scaledWidth, scaledHeight);
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