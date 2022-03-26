#include <iostream>

#pragma once

// Color class.
class REGL_Color{
    public:
        int r, g, b, a;
        REGL_Color(int r, int g, int b, int a);
        REGL_Color() = default;
};

REGL_Color::REGL_Color(int r, int g, int b, int a = 255){
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    if (r == 0 && g == 0 && b == 0){
        this->b = 1;
    }
}