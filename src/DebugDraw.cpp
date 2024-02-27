#include "DebugDraw.h"

#include <vector>

#include <SDL.h>
#include <SDL_render.h>

#include "Camera.h"

DebugDraw::DebugDraw() {
    // rgba range 0-1
    m_renderColor = ImVec4(0.0f, 1.0f, 1.0f, 1.00f);
}

DebugDraw::~DebugDraw() {}



int SDL_RenderDrawCircle(SDL_Renderer * renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y + offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y + offsetx);
        status += SDL_RenderDrawPoint(renderer, x + offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x + offsety, y - offsetx);
        status += SDL_RenderDrawPoint(renderer, x - offsetx, y - offsety);
        status += SDL_RenderDrawPoint(renderer, x - offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

int SDL_RenderFillCircle(SDL_Renderer * renderer, int x, int y, int radius) {
    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx,
                                     x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety,
                                     x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety,
                                     x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx,
                                     x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2*offsetx) {
            d -= 2*offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

    return status;
}

void DebugDraw::Init(SDL_Renderer* m_SDL_Renderer) {
    this->m_SDL_Renderer = m_SDL_Renderer;
}

void DebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    for(int i = 1; i <= vertexCount; i++) {
        if(i == vertexCount) {
            DrawSegment(vertices[0], vertices[i - 1], color);
            break;
        }
        DrawSegment(vertices[i - 1], vertices[i], color);
    }
}

void DebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) {
    // TODO: This is a temp code.
    DrawPolygon(vertices, vertexCount, color);
}

void DebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color) {
    SetRenderColor(color);
    auto ps = g_camera.ConvertWorldToScreen(center);
    SDL_RenderDrawCircle(m_SDL_Renderer, static_cast< int >(ps.x), static_cast< int >(ps.y), g_camera.ConvertWorldToScreen(radius));
}

void DebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color) {
    SetRenderColor(color);
    auto ps = g_camera.ConvertWorldToScreen(center);
    SDL_RenderFillCircle(m_SDL_Renderer, static_cast<int>(ps.x), static_cast<int>(ps.y), g_camera.ConvertWorldToScreen(radius));
}

void DebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) {
    SetRenderColor(color);

    auto ps1 = g_camera.ConvertWorldToScreen(p1);
    auto ps2 = g_camera.ConvertWorldToScreen(p2);

    SDL_RenderDrawLine(m_SDL_Renderer, static_cast< int >(ps1.x), static_cast< int >(ps1.y), static_cast< int >(ps2.x), static_cast< int >(ps2.y));
}

void DebugDraw::DrawTransform(const b2Transform& xf) {


}

void DebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color) {
    SetRenderColor(color);
    auto ps = g_camera.ConvertWorldToScreen(p);
    SDL_RenderDrawPoint(m_SDL_Renderer, static_cast< int >(ps.x), static_cast< int >(ps.y));
}

void DebugDraw::SetRenderColor(const b2Color& color) {
    SDL_Color sdl_color{( Uint8 )(color.r * 255), ( Uint8 )(color.g * 255), ( Uint8 )(color.b * 255), ( Uint8 )(color.a * 255)};

    SDL_SetRenderDrawColor(m_SDL_Renderer, sdl_color.r, sdl_color.g, sdl_color.b, sdl_color.a);
}

void DebugDraw::DrawString(int x, int y, const char* string, ...)
{
	// if (m_showUI == false)
	// {
	// 	return;
	// }

	va_list arg;
	va_start(arg, string);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground);
	ImGui::SetCursorPos(ImVec2(float(x), float(y)));
	ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
	ImGui::End();
	va_end(arg);
}

void DebugDraw::DrawString(const b2Vec2& pw, const char* string, ...)
{
	b2Vec2 ps = g_camera.ConvertWorldToScreen(pw);

	va_list arg;
	va_start(arg, string);
	ImGui::Begin("Overlay", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
	ImGui::SetCursorPos(ImVec2(ps.x, ps.y));
	ImGui::TextColoredV(ImColor(230, 153, 153, 255), string, arg);
	ImGui::End();
	va_end(arg);
}

// TODO: This seems to be not used
void DebugDraw::DrawAABB(b2AABB* aabb, const b2Color& c)
{
	b2Vec2 p1 = aabb->lowerBound;
	b2Vec2 p2 = b2Vec2(aabb->upperBound.x, aabb->lowerBound.y);
	b2Vec2 p3 = aabb->upperBound;
	b2Vec2 p4 = b2Vec2(aabb->lowerBound.x, aabb->upperBound.y);

    SetRenderColor(c);

    DrawSegment(p1, p2, c);
    DrawSegment(p2, p3, c);
    DrawSegment(p3, p4, c);
    DrawSegment(p4, p1, c);
}
