#include <Box2D/Box2D.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <memory>

#include "Application.h"

b2World* world;

const int MET2PIX = 100;  // 640 / 80 = 8

const int WIDTH  = 1000;
const int HEIGHT = 700;

const int SCALED_WIDTH  = WIDTH / MET2PIX;  // 4 | 3
const int SCALED_HEIGHT = HEIGHT / MET2PIX;

// 1 rad × 180/π = 57,296°
const float RAD2DEG = 180 / M_PI;

template <typename T>
void print(T value) {
    std::cout << value << std::endl;
}

void createBar(float ground_x, float ground_y, b2Vec2 point1, b2Vec2 point2) {
    b2BodyDef BarDef;
    BarDef.type = b2_staticBody;
    BarDef.position.Set(ground_x, ground_y);
    BarDef.angle        = 0;
    b2Body*     BarBody = world->CreateBody(&BarDef);

    b2EdgeShape BarShape;
    BarShape.SetTwoSided(point1, point2);

    b2FixtureDef BarFixtureDef;
    BarFixtureDef.shape = &BarShape;
    BarBody->CreateFixture(&BarFixtureDef);
}

int main() {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);
    auto Width  = DM.w;
    auto Height = DM.h;

    std::cout << "Width of the Screen: " << Width << "px" << std::endl;
    std::cout << "Height of the Screen: " << Height << "px" << std::endl;

    SDL_Window* window = SDL_CreateWindow("FirstGame", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if(window == NULL) {
        print("SDL window failed to initialize! ");
        return -1;
    }
    print("SDL window initialized.");

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    world = new b2World(b2Vec2(0.0f, 9.81f));  // new b2World(b2Vec2(0.0f, 9.81f));
    // auto world = std::make_shared<b2Vec2>(b2Vec2(0.0f, 9.81f));

    // >>>>>>>>>>> window and world initialization finished
    // cartesian origin
    float ground_x = -0.0f;
    float ground_y = -0.0f;

    // start ground point
    b2Vec2 startpoint;
    startpoint.x = -3.0f;
    startpoint.y = 2.0;

    // end ground point
    b2Vec2 endpoint;
    endpoint.x = 3.0;
    endpoint.y = 2.0;

    // LineGround
    // b2BodyDef myGroundDef;
    // myGroundDef.type = b2_staticBody;
    // myGroundDef.position.Set(ground_x, ground_y);  // set the starting position x and y cartesian
    // myGroundDef.angle = 0;
    // b2Body* groundLineBody = world->CreateBody(&myGroundDef);

    b2EdgeShape edgeShape; 
    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector

    // b2FixtureDef edgeFixtureDef;
    // edgeFixtureDef.shape = &edgeShape;
    // groundLineBody->CreateFixture(&edgeFixtureDef);
    createBar(ground_x, ground_y, startpoint, endpoint);
    // mycode
    // right bar
    b2Vec2 verticalDelta {0.0f, 5.0f};
    createBar(-0.0f, -0.0f, endpoint, endpoint - verticalDelta);
    // right bar end
    // left bar
    createBar(-0.0f, -0.0f, startpoint, startpoint - verticalDelta);

    // mycode end

    // >> load the box figure
    SDL_Surface* tmp_sprites;
    tmp_sprites = IMG_Load("assets/box.png");
    if(!tmp_sprites)
        return EXIT_FAILURE;

    SDL_Texture* texture_box = SDL_CreateTextureFromSurface(renderer, tmp_sprites);
    SDL_FreeSurface(tmp_sprites);
    // << end loading

    // cartesian origin box
    float x_box = -0.0f;
    float y_box = -0.0f;

    // size of box
    float w_box = 0.3;
    float h_box = 0.3;

    // angle of the box
    float angle_box = 45.0f;  // 45.0f;

    // Box
    SDL_Rect box;
    b2Body*  Body;

    b2BodyDef boxBodyDef;
    boxBodyDef.type  = b2_dynamicBody;
    boxBodyDef.angle = angle_box;  // flips the whole thing -> 180 grad drehung
    // boxBodyDef.angle = 0;
    boxBodyDef.position.Set(x_box, y_box);

    // velocity created here
    b2Vec2 vel{ 1.0f, 0.2f };

    Body = world->CreateBody(&boxBodyDef);
    Body->SetLinearVelocity(vel);

    b2PolygonShape dynamicBox;
    // xxx
    dynamicBox.SetAsBox((w_box / 2.0f) - dynamicBox.m_radius, (h_box / 2.0f) - dynamicBox.m_radius);  // will be 0.5 x 0.5
    // dynamicBox.SetAsBox((w_box / 2.0f) - 0.0f, (h_box / 2.0f) - 0.0f);  // will be 0.5 x 0.5
    std::cout << "dynamicBox.SetAsBox" <<(w_box / 2.0f) - dynamicBox.m_radius << ' ' << (h_box / 2.0f) - dynamicBox.m_radius <<std::endl;

    b2FixtureDef fixtureDef;
    fixtureDef.shape       = &dynamicBox;
    fixtureDef.density     = 1;
    fixtureDef.friction    = 0.3f;
    fixtureDef.restitution = 1.0f;  // modified
    Body->CreateFixture(&fixtureDef);

    // box: convert Metres back to Pixels for width and height
    box.w = w_box * MET2PIX;
    box.h = h_box * MET2PIX;
}