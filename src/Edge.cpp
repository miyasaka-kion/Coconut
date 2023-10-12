#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Edge.h"
#include "Constants.h"
#include "MetricConverter.h"

Edge::Edge(b2World* world, SDL_Renderer* renderer) : m_world(world), m_renderer(renderer) {
    // some constants
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -3.0f;
    startpoint.y = -2.0;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3.0;
    endpoint.y = -2.0;
    // constants end

    // LineGround
    b2BodyDef myGroundDef;
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y);  // set the starting position x and y cartesian
    myGroundDef.angle = 0;

    body = m_world->CreateBody(&myGroundDef);

    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector

    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.shape = &edgeShape;
    body->CreateFixture(&edgeFixtureDef);

    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector
}

Edge::~Edge() {
    m_world->DestroyBody(body); // seems this is not needed? should check the source code.
}

void Edge::render() {
    SDL_RenderDrawLine(m_renderer, 
    MetricConverter::toPixX(edgeShape.m_vertex1.x),
    MetricConverter::toPixY(edgeShape.m_vertex1.y),
    MetricConverter::toPixX(edgeShape.m_vertex2.x), 
    MetricConverter::toPixY(edgeShape.m_vertex2.y));
}



