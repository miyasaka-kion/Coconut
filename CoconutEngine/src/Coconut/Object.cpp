#include "Object.h"

Coconut::Object::Object(std::string textureSheet) {
	m_objectTexture = Coconut::TextureManager::LoadTexture(textureSheet);
}

Coconut::Object::Object(std::string textureSheet, int ix, int iy) {
	m_xpos = ix;
	m_ypos = iy;
	m_objectTexture = Coconut::TextureManager::LoadTexture(textureSheet);
}

Coconut::Object::~Object() {
}

void Coconut::Object::objUpdate() {
	m_xpos += 1;
	m_ypos += 1;


	m_srcRect.h = 630;
	m_srcRect.w = 765;
	m_srcRect.x = 0;
	m_srcRect.y = 0;

	m_destRect.h = 64;
	m_destRect.w = 64;
	m_destRect.x = m_xpos;
	m_destRect.y = m_ypos;
}

void Coconut::Object::objRender() {
	SDL_RenderCopy(Coconut::Game::renderer, m_objectTexture, &m_srcRect, &m_destRect);

}
