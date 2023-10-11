#pragma once
#include <box2d/box2d.h>

class Application {
public:
    Application();
    ~Application();
    
    void run();

    void init();  
    void update();
    
    void loadBoundary();
    void loadEntity();
private:
    void createBar(float ground_x, float ground_y, b2Vec2 point1, b2Vec2 point2);
    
public: 
    b2World* world;
};