#pragma once 

struct ccVec2 {
    float x;
    float y;
};


struct ccCol {
    ccCol() : r(0.0f), g(0.0f), b(0.0f), a(0.0f) {}
    ccCol(float _r, float _g, float _b, float _a) : r(_r), g(_g), b(_b), a(_a) {}

    void Set(float _r, float _g, float _b, float _a) {
        r = _r;
        g = _g;
        b = _b;
        a = _a;
    }

    

    float r;
    float g;
    float b;
    float a;
};
