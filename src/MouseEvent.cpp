#include "MouseEvent.h"

#include "Camera.h"
#include "Log.h"

void MouseEvent::MouseDown(b2Vec2& pw) {
    CC_CORE_INFO("mouse down");
    m_mouseDown = true;
}

void MouseEvent::MouseUp(b2Vec2& pw) {
    CC_CORE_INFO("mouse up");
    m_mouseDown = false;
}

void MouseEvent::RightMouseDown(b2Vec2& pw) {
    CC_CORE_INFO("right mouse down");
    m_mouseClickPointW = pw;
    m_rightMouseDown   = true;
}

void MouseEvent::RightMouseUp(b2Vec2& pw) {
    CC_CORE_INFO("right mouse up");
    m_rightMouseDown = false;
}

// this is a weird way of dealing with mouse motion
// the argument is not elegant
void MouseEvent::MouseMotion(b2Vec2& ps) {
    if(m_rightMouseDown) {
        CC_CORE_INFO("Mouse motion triggered");
        auto   pw   = g_camera.ConvertScreenToWorld(ps);
        b2Vec2 diff = pw - m_mouseClickPointW;
        g_camera.m_center.x -= diff.x;
        g_camera.m_center.y -= diff.y;
        m_mouseClickPointW = g_camera.ConvertScreenToWorld(ps);
    }
}

void MouseEvent::MouseWheelDown(b2Vec2& pw) {
    g_camera.m_zoom /= 1.1f;
}

void MouseEvent::MouseWheelUp(b2Vec2& pw) {

    g_camera.m_zoom *= 1.1f;
}