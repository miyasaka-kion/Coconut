#pragma once

#include <box2d/box2d.h>

class Camera {
public:
    Camera();

    void   ResetView();
    b2Vec2 ConvertScreenToWorld(const b2Vec2& screenPoint);
    b2Vec2 ConvertWorldToScreen(const b2Vec2& worldPoint);
    void   BuildProjectionMatrix(float* m, float zBias);

    float ConvertScreenToWorld(int length);
    int   ConvertWorldToScreen(float length);

    b2Vec2 m_center;
    float  m_zoom;
    int32  m_width;
    int32  m_height;
};

extern Camera g_camera;