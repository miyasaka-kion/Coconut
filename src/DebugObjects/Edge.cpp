#include "Edge.h"

#include <tuple>

#include <SDL_render.h>
#include <box2d/box2d.h>

#include "../Camera.h"
#include "../Constants.h"
#include "../Entity.h"



Edge::Edge(b2World* world, SDL_Renderer* renderer) : Entity(world, renderer) {}

Edge::~Edge() {
    m_world->DestroyBody(body); // seems this is not needed? should check the source code.
}

void Edge::Init(b2Vec2 startpoint, b2Vec2 endpoint) {
    // LineGround
    b2BodyDef myGroundDef;
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(c_GroundX, c_GroundY);  // set the starting position x and y cartesian
    myGroundDef.angle = 0;

    body = m_world->CreateBody(&myGroundDef);

    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector

    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.shape = &edgeShape;
    body->CreateFixture(&edgeFixtureDef);

    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector
}

void Edge::Render() {
    SDL_SetRenderDrawColor(m_renderer, 255, 255, 0, 0);

    auto p1 = g_camera.ConvertWorldToScreen(edgeShape.m_vertex1);
    auto p2 = g_camera.ConvertWorldToScreen(edgeShape.m_vertex2);
    
    int x1, y1, x2, y2;
    std::tie(x1, y1) = std::make_tuple(static_cast<int>(p1.x), static_cast<int>(p1.y));
    std::tie(x2, y2) = std::make_tuple(static_cast<int>(p2.x), static_cast<int>(p2.y));

    SDL_RenderDrawLine(m_renderer, x1, y1, x2, y2);
}



