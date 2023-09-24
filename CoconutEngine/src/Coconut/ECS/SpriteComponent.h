// This is a personal academic project. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#pragma once

#include <string>

#include "Coconut/Log.h"
#include "Coconut/ECS/ECS.h"
#include "Coconut/TextureManager.h"
#include "Coconut/ECS/TransformComponent.h"
#include "Coconut/ConstantSpec.h"



namespace Coconut {
	class SpriteComponent : public Coconut::Component {
	private:
		Coconut::TransformComponent* m_transform;
		SDL_Texture* m_texture;
		SDL_Rect m_srcRect, m_destRect;

	public:

		SpriteComponent() = default;
		SpriteComponent(std::string path) {
			setTexture(path);
		}

		~SpriteComponent() {
			SDL_DestroyTexture(m_texture);
		}

		void setTexture(std::string path) {
			m_texture = Coconut::TextureManager::LoadTexture(path);
		}

		void init() override {
			m_transform = &entity->getComponent<TransformComponent>();
			m_srcRect.x = m_srcRect.y = 0;
			m_srcRect.w = m_transform->width;
			m_srcRect.h = m_transform->hight;
			
			m_destRect.w = m_transform->width * m_transform->scale;
			m_destRect.w = m_destRect.h = Coconut::Constants::DEST_RECT_HEIGHT;

			CC_CORE_INFO("SpriteComponent initialized!");
		}

		void update() override {
			std::tie(m_destRect.x, m_destRect.y) = m_transform->getCoordinate();
		}
		
		void draw() override {
			Coconut::TextureManager::DrawTexture(m_texture, m_srcRect, m_destRect);
		}
	};
}
