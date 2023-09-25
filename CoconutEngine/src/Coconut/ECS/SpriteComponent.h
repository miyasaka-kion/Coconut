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

		int width;
		int height;
	public:

		SpriteComponent() = default;
		SpriteComponent(std::string path) {
			// Some weird things happed here
			// emmm
			// 
			//if (entity->hasComponent<TransformComponent>() == false) {
			//	CC_CORE_INFO("Coconut Engine dectect that the entity has SpriteComponent, but do not have an TransformComponent.");
			//	CC_CORE_INFO("Coconut add an TransformComponent for that entity.");
			//	entity->addComponent<TransformComponent>();
			//}
			//m_transform = &entity->getComponent<Coconut::TransformComponent>();

			setTexture(path);
		}

		~SpriteComponent() {
			SDL_DestroyTexture(m_texture);
		}

		void setTexture(std::string path) {
			// Attention: transform->width and transfrom->height is actually initialized here!!!
			std::tie(m_texture, width, height) = Coconut::TextureManager::LoadTexture_tuple(path);
		}

		void init() override {
			m_transform = &entity->getComponent<Coconut::TransformComponent>();
			// this should be optimized!
			m_transform->width = width;
			m_transform->height = height;
			// <<<<
			m_srcRect.x = m_srcRect.y = 0;
			m_srcRect.w = m_transform->width;
			m_srcRect.h = m_transform->height;
			
			std::tie(m_destRect.w, m_destRect.h) = m_transform->getScaledSize();
			CC_CORE_INFO("SpriteComponent initialized!");
		}

		void update() override {
			std::tie(m_destRect.w, m_destRect.h) = m_transform->getScaledSize();
			std::tie(m_destRect.x, m_destRect.y) = m_transform->getCoordinate();
		}
		
		void draw() override {
			Coconut::TextureManager::DrawTexture(m_texture, m_srcRect, m_destRect);
		}
	};
}
