#pragma once 

#include <box2d/box2d.h>

class MouseEvent {
public:
    b2Vec2 m_mouseClickPointW;
    bool m_mouseDown = false;
    bool m_rightMouseDown = false;
    virtual void MouseDown(b2Vec2& pw);
    virtual void MouseUp(b2Vec2& pw);
    // virtual void ShiftMouseDown();
    // virtual void ShiftMouseUp(); TODO

    virtual void RightMouseDown(b2Vec2& pw);
    virtual void RightMouseUp(b2Vec2& pw);

    virtual void MouseMotion(b2Vec2& pw);

    virtual void MouseWheelUp(b2Vec2& pw);
    virtual void MouseWheelDown(b2Vec2& pw);
};