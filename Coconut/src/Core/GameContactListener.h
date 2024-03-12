#pragma once

#include <box2d/box2d.h>

class GameContactListener : public b2ContactListener {
    void BeginContact(b2Contact* contact) override {
        b2Fixture* fixtureA = contact->GetFixtureA();
        b2Fixture* fixtureB = contact->GetFixtureB();
        
    }

    void EndContact(b2Contact* contact) override {

    }
};


