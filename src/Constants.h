#pragma once
#include <box2d/box2d.h>

constexpr int MET2PIX = 100;  // 640 / 80 = 8

constexpr int WIDTH  = 1000;
constexpr int HEIGHT = 700;

constexpr int SCREEN_WIDTH_IN_METER  = WIDTH / MET2PIX;  // 4 | 3
constexpr int SCREEN_HEIGHT_IN_METER = HEIGHT / MET2PIX;

// 1 rad × 180/π = 57,296°
constexpr float RAD2DEG = 180 / M_PI;

// some default values
constexpr float c_originalAngle = M_PI / 5;

// cartesian origin box
constexpr float c_xOriginPosition = -0.0f;
constexpr float c_yOriginPosition = -0.0f;
const b2Vec2 c_OriginPos {c_xOriginPosition, c_yOriginPosition};

// size of box
constexpr float c_OriginalBoxWidth = 0.3f;
constexpr float c_OriginalBoxHeight = 0.3f;


constexpr float c_GroundX = -0.0f;
constexpr float c_GroundY = -0.0f;

// extern b2Vec2 vel;
// this should be removed in the future
// b2Vec2 vel{ 1.0f, -0.1f };
const b2Vec2 c_OriginalVelocity{ 0.0f, -0.1f }; 
const b2Vec2 C_OriginPos {0.0f, 0.0f};