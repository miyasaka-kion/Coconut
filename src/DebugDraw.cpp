#include "DebugDraw.h"
#include "Camera.h"

#include <SDL.h>
#include <SDL_render.h>

DebugDraw::DebugDraw() {
    // rgba range 0-1
    m_renderColor = ImVec4(0.0f, 1.0f, 01.0f, 1.00f);
}

DebugDraw::~DebugDraw() {}

void DebugDraw::Init(SDL_Renderer* m_SDL_Renderer) {
    this->m_SDL_Renderer = m_SDL_Renderer;
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
//     for(int i = 0; i < vertexCount; i++) {
        
//     }
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {

}

void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {


}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {


}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    SDL_SetRenderDrawColor(m_SDL_Renderer, ( Uint8 )(m_renderColor.x * 255), ( Uint8 )(m_renderColor.y * 255), ( Uint8 )(m_renderColor.z * 255), ( Uint8 )(m_renderColor.w * 255));

    auto ps1 = g_camera.ConvertWorldToScreen(p1);
    auto ps2 = g_camera.ConvertWorldToScreen(p2);

    SDL_RenderDrawLine(m_SDL_Renderer, static_cast< int >(ps1.x), static_cast< int >(ps1.y), static_cast< int >(ps2.x), static_cast< int >(ps2.y));
}

void DebugDraw::DrawTransform(const b2Transform& xf) {

    
}

void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
    SDL_SetRenderDrawColor(m_SDL_Renderer, ( Uint8 )(m_renderColor.x * 255), ( Uint8 )(m_renderColor.y * 255), ( Uint8 )(m_renderColor.z * 255), ( Uint8 )(m_renderColor.w * 255));
    auto ps = g_camera.ConvertWorldToScreen(p);
    SDL_RenderDrawPoint(m_SDL_Renderer, static_cast< int >(ps.x), static_cast< int >(ps.y));
}