#pragma once

#include <box2d/box2d.h>

const int32 k_maxContactPoints = 2048;

struct ContactPoint {
    b2Fixture*   fixtureA;
    b2Fixture*   fixtureB;
    b2Vec2       normal;
    b2Vec2       position;
    b2PointState state;
    float        normalImpulse;
    float        tangentImpulse;
    float        separation;
};


class PhysicsInfo {
public:
    int           m_pointCount;
    b2MouseJoint* m_mouseJoint;  // TODO: how to use this?
    b2Profile     m_maxProfile;
    b2Profile     m_totalProfile;
    int           m_stepCount;
    ContactPoint  m_points[k_maxContactPoints];
};
