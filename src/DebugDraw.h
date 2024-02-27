#pragma once

#include <SDL_render.h>
#include <box2d/box2d.h>
#include "imgui.h"

class DebugDraw : public b2Draw {
public:
    DebugDraw();
    ~DebugDraw();

    void Init(SDL_Renderer*);

    // void Create();
    // void Destroy();

    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override;

    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color) override;

    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) override;

    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override;

    void DrawTransform(const b2Transform& xf) override;

    void DrawPoint(const b2Vec2& p, float size, const b2Color& color) override;

    void DrawString(int x, int y, const char* string, ...);

    void DrawString(const b2Vec2& pw, const char* string, ...); // TODO : Something weird when rendering

    void DrawAABB(b2AABB* aabb, const b2Color& color);

private:
    void SetRenderColor(const b2Color& color);

private:
    SDL_Renderer* m_SDL_Renderer;
    ImVec4        m_renderColor;

    // int m_textLine;
};