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

    b2EdgeShape edgeShape; 
    edgeShape.SetTwoSided(startpoint, endpoint);  // length -> coordinate vector from to vector

    // groundLineBody->CreateFixture(&edgeFixtureDef);
    createBar(ground_x, ground_y, startpoint, endpoint);
    b2Vec2 verticalDelta {0.0f, 5.0f};
    createBar(-0.0f, -0.0f, endpoint, endpoint - verticalDelta);
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

    bool      close_game = false;
    SDL_Event event;

    // The game Loop
    while(close_game != true) {
        b2Vec2 pos   = Body->GetPosition();  // Body = Body from box
        float  angle = Body->GetAngle();

        // RAD2Degree
        angle *= RAD2DEG;

        while(SDL_PollEvent(&event)) {
            if(event.type == SDL_QUIT)
                close_game = true;

            else if(event.key.keysym.sym == SDLK_ESCAPE)
                close_game = true;

            else if(event.key.keysym.sym == SDLK_r) {
                Body->SetTransform(b2Vec2(x_box, y_box), angle_box);
                Body->SetLinearVelocity(vel);
            }
        }

        // question box, update x and y destination
        box.x = ((SCALED_WIDTH / 2.0f) + pos.x) * MET2PIX - box.w / 2;
        box.y = ((SCALED_HEIGHT / 2.0f) + pos.y) * MET2PIX - box.h / 2;

        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);

        // Draw ground platform
        // SDL_SetRenderDrawColor(renderer, 255, 255, 0, 0);
        SDL_RenderDrawLine(renderer, ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex1.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex1.y) * MET2PIX,
                           ((SCALED_WIDTH / 2.0f) + edgeShape.m_vertex2.x) * MET2PIX, ((SCALED_HEIGHT / 2.0f) + edgeShape.m_vertex2.y) * MET2PIX);

        SDL_RenderCopyEx(renderer, texture_box, NULL, &box, Body->GetAngle() * RAD2DEG, NULL, SDL_FLIP_NONE);


        SDL_SetRenderDrawColor(renderer, 32, 70, 49, 0);
        SDL_RenderPresent(renderer);

        world->Step(1.0f / 60.0f, 6.0f, 2.0f);  // update
    }

    // box2D delete whole world and free memory
    delete world;

    SDL_DestroyTexture(texture_box);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return EXIT_SUCCESS;
}