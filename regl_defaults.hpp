#include <iostream>
#include <unordered_map>

#include "color.hpp"

#include "SDL2/include/SDL2/SDL.h"

#pragma once

// REGL renderer flags.
#define REGL_RENDER_GPU SDL_RENDERER_ACCELERATED
#define REGL_RENDER_SOFTWARE SDL_RENDERER_SOFTWARE

// Rect that fills everything (cropping).
SDL_Rect _FILL_RECT_ALL = {0, 0, 10000, 10000};

// Let the child object decide the nextline.
int REGL_NEXTLINE_DEFAULT = 1294787;

// Default scene.
std::string REGL_SCENE_DEFAULT = "default";

// Builtin Colors.
REGL_Color REGL_COLOR_WHITE = REGL_Color(255, 255, 255);
REGL_Color REGL_COLOR_BLACK = REGL_Color(0, 0, 0);
REGL_Color REGL_COLOR_RED = REGL_Color(255, 0, 0);
REGL_Color REGL_COLOR_ORANGE = REGL_Color(255, 140, 0);
REGL_Color REGL_COLOR_YELLOW = REGL_Color(255, 255, 0);
REGL_Color REGL_COLOR_BLUE = REGL_Color(0, 0, 255);
REGL_Color REGL_COLOR_GREEN = REGL_Color(0, 255, 0);
REGL_Color REGL_COLOR_PURPLE = REGL_Color(230, 230, 250);
REGL_Color REGL_COLOR_GREY = REGL_Color(200, 200, 200);