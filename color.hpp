#include <iostream>

#pragma once

// Color class.
class CUI_Color{
    public:
        int r, g, b, a;
        CUI_Color(int r, int g, int b, int a);
        CUI_Color() = default;
};

CUI_Color::CUI_Color(int r, int g, int b, int a = 255){
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
    if (r == 255 && g == 255 && b == 255){
        this->b = 254;
    }
}