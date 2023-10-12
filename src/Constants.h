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
constexpr float angle_box = 45.0f;

// cartesian origin box
constexpr float x_box = -0.0f;
constexpr float y_box = -0.0f;

// size of box
constexpr float w_box = 0.3;
constexpr float h_box = 0.3;

constexpr float ground_x = -0.0f;
constexpr float ground_y = -0.0f;

// extern b2Vec2 vel;
// this should be removed in the future
// b2Vec2 vel{ 1.0f, -0.1f };
const b2Vec2 vel{ 1.0f, -0.1f }; 