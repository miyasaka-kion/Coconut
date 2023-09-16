#pragma once

#include <string>

#include "SDL.h"

#include "Compoent.h"

namespace Coconut {
	class SpriteComponent : public Coconut::Component {
	private:
		Coconut::PositionComponent* position;
		SDL_Texture* texture;
		SDL_Rect srcRect, destRect;

	public:

		SpriteComponent() = default;
		SpriteComponent (std::string path) {
			texture = Coconut::TextureManager::LoadTexture(path);
		}

		void init() override {
			position = &entity->getComponent<PositionComponent>();
			srcRect.x = srcRect.y = 0;
			//srcRect.w = srcRect.h = 32;
			srcRect.w = 765;
			srcRect.h = 630;
			
			destRect.w = destRect.h = 64;


		}

		void update() override {
			destRect.x = position->getXpos();
			destRect.y = position->getYpos();

		}
		
		void draw() override {
			Coconut::TextureManager::DrawTexture(texture, srcRect, destRect);
		}
	};
}
