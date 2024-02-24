#pragma once 

#include <box2d/box2d.h>

class MouseEvent {
private:
    b2Vec2 m_mouseClickPointW;
    b2Vec2 m_oldCameraCenter;
    bool m_mouseDown;
    bool m_rightMouseDown;
    virtual void MouseDown(b2Vec2& pw);
    virtual void MouseUp(b2Vec2& pw);
    // // virtual void ShiftMouseDown();
    // // virtual void ShiftMouseUp(); TODO

    virtual void RightMouseDown(b2Vec2& pw);
    virtual void RightMouseUp(b2Vec2& pw);
};