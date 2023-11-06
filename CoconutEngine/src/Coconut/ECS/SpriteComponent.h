#pragma once

#include <string>
#include <tuple>

#include "Coconut/ConstantSpec.h"
#include "Coconut/ECS/BodyComponent.h"
#include "Coconut/ECS/ECS.h"
#include "Coconut/ECS/TransformComponent.h"
#include "Coconut/Log.h"
#include "Coconut/TextureManager.h"
#include "Coconut/MetricConverter.h"

namespace Coconut {
class SpriteComponent : public Coconut::Component {
private:
    // Coconut::TransformComponent* m_transform;
    Coconut::BodyComponent*      m_body;
    SDL_Texture*                 m_texture;
    SDL_Rect                     m_srcRect, m_destRect;

    // this is the size info of the sprite picture
    int width;
    int height;

public:
    SpriteComponent() = default;
    SpriteComponent(std::string path) {
        setTexture(path);
    }

    ~SpriteComponent() {
        SDL_DestroyTexture(m_texture);
    }

    void setTexture(std::string path) {
        std::tie(m_texture, width, height) = Coconut::TextureManager::LoadTexture_tuple(path);
        CC_INFO("Texture set, path = {}", path.c_str());
    }

    void init() override {
        // m_transform = &entity->getComponent<Coconut::TransformComponent>();
        m_body = &entity->getComponent<Coconut::BodyComponent>(); // Question: Why this cannot be written in the 
        m_srcRect.x = 0;
        m_srcRect.y = 0;
        m_srcRect.w = width;
        m_srcRect.h = height;

        // the render size here should be determined by some feature of the body component
        // this is just a temporary code
        // m_destRect.w = width * m_transform->scale;
        // m_destRect.h = height * m_transform->scale;
        
        std::tie(m_destRect.x, m_destRect.y) = MetricConverter::toPixCoordinate(m_body->getPosition());
        CC_CORE_INFO("SpriteComponent initialized!");
    }

    void update() override {
        // the render size here should be determined by some feature of the body component
        // this is just a temporary code
        // m_destRect.w = width * m_transform->scale;
        // m_destRect.h = height * m_transform->scale;
        std::tie(m_destRect.x, m_destRect.y) = MetricConverter::toPixCoordinate(m_body->getPosition());
    }

    void render() override {
        // Coconut::TextureManager::DrawTexture(m_texture, m_srcRect, m_destRect);
        Coconut::TextureManager::DrawTexture(m_texture, m_destRect);
    }
};
}  // namespace Coconut
