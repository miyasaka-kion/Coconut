#pragma once
#include <box2d/box2d.h>

constexpr int c_pixelPerMeter = 100;  // 640 / 80 = 8

// constexpr int c_screenWidthPix  = 1000;
// constexpr int c_screenHeightPix = 700;

// constexpr float c_screenWidthMeter  = static_cast<float>(c_screenWidthPix) / c_pixelPerMeter;  // 4 | 3
// constexpr float c_screenHeightMeter = static_cast<float>(c_screenHeightPix) / c_pixelPerMeter;

// constexpr float c_xBoundary = c_screenWidthMeter / 2.0f;
// constexpr float c_yBoundary = c_screenHeightMeter / 2.0f;

// 1 rad × 180/π = 57,296°
constexpr float c_degreePerRad = 180 / M_PI;

// some default values
constexpr float c_originalAngle = M_PI / 5;

// cartesian origin box
constexpr float c_xOriginPosition = -0.0f;
constexpr float c_yOriginPosition = -0.0f;
const b2Vec2    c_OriginPos{ c_xOriginPosition, c_yOriginPosition };

// size of box
constexpr float c_OriginalBoxWidth  = 1.5f;
constexpr float c_OriginalBoxHeight = 1.5f;

constexpr float c_GroundX = -0.0f;
constexpr float c_GroundY = -0.0f;

const b2Vec2 c_OriginalVelocity{ 0.0f, -0.1f };