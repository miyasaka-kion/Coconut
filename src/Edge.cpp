#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Edge.h"
#include "Constants.h"
#include "Entity.h"
#include "MetricConverter.h"



// Edge::Edge(b2World* world, SDL_Renderer* renderer, b2Vec2 startpoint, b2Vec2 endpoint) : Entity(world, renderer) {

//     // LineGround
//     b2BodyDef myGroundDef;
//     myGroundDef.type = b2_staticBody;
//     myGroundDef.position.Set(c_GroundX, c_GroundY);  // set the starting position x and y cartesian
//     myGroundDef.angle = 0;

//     body = m_world->CreateBody(&myGroundDef);

//     edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector

//     b2FixtureDef edgeFixtureDef;
//     edgeFixtureDef.shape = &edgeShape;
//     body->CreateFixture(&edgeFixtureDef);

//     edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector
// }

Edge::Edge(b2World* world, SDL_Renderer* renderer) : Entity(world, renderer) {}

Edge::~Edge() {
    m_world->DestroyBody(body); // seems this is not needed? should check the source code.
}

void Edge::init(b2Vec2 startpoint, b2Vec2 endpoint) {
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

void Edge::render() {
    SDL_RenderDrawLine(m_renderer, 
    MetricConverter::toPixX(edgeShape.m_vertex1.x),
    MetricConverter::toPixY(edgeShape.m_vertex1.y),
    MetricConverter::toPixX(edgeShape.m_vertex2.x), 
    MetricConverter::toPixY(edgeShape.m_vertex2.y));
}



