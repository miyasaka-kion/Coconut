#pragma once
#include "Coconut/Game.h"
#include "Coconut/TextureManager.h"

#pragma warning(push, 0)
#include <SDL.h>
#include <SDL_image.h>
#pragma warning(pop)

namespace Coconut {
class Object {
public:
    Object(std::string textureSheet);
    Object(std::string textureSheet, int ix, int iy);
    //(ix, iy) is the initial position of the object
    ~Object();

    void objUpdate();
    void objRender();

private:
    int m_xpos;
    int m_ypos;

    SDL_Texture* m_objectTexture;
    SDL_Rect     m_srcRect;
    SDL_Rect     m_destRect;
};
}  // namespace Coconut