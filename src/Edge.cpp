#include <SDL_render.h>
#include <box2d/box2d.h>

#include "Edge.h"
#include "Constants.h"

Edge::Edge(b2World* world, SDL_Renderer* renderer) : m_world(world), m_renderer(renderer) {
    // some constants
    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -3.0f;
    startpoint.y = 2.0;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3.0;
    endpoint.y = 2.0;
    // constants end

    // LineGround
    b2BodyDef myGroundDef;
    myGroundDef.type = b2_staticBody;
    myGroundDef.position.Set(ground_x, ground_y);  // set the starting position x and y cartesian
    myGroundDef.angle = 0;

    groundLineBody = m_world->CreateBody(&myGroundDef);

    b2EdgeShape edgeShape;
    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector

    b2FixtureDef edgeFixtureDef;
    edgeFixtureDef.shape = &edgeShape;
    groundLineBody->CreateFixture(&edgeFixtureDef);

    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector
}

Edge::~Edge() {
    m_world->DestroyBody(groundLineBody); // seems this is not needed? should check the source code.
}

void Edge::render(SDL_Renderer* renderer) {
    SDL_RenderDrawLine(renderer, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex1.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex1.y) * MET2PIX,
                           ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex2.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex2.y) * MET2PIX);
}

