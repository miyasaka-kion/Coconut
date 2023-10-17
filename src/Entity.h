#pragma once

class Entity {
public:
    Entity() = default;
    virtual ~Entity() {}
    virtual void render() = 0;

    bool isActive = 1;
};