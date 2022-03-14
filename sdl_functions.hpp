#include <iostream>
#include <unordered_map>

#include "color.hpp"

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"

#pragma once

std::unordered_map<std::string, SDL_Texture*> _cached_textures;

// Load a texture and cache it.
SDL_Texture* loadTexture(SDL_Renderer* renderer, std::string path){

    if (_cached_textures.count(path) != 0){
        return _cached_textures[path];
    }

    SDL_Surface* temp_surface = IMG_Load(path.c_str());

    if (!temp_surface){
        std::cout << "Unable to load image: " << SDL_GetError() << "\n";
    }

    SDL_Texture* loaded_texture = SDL_CreateTextureFromSurface(renderer, temp_surface);
    SDL_FreeSurface(temp_surface);

    // cache texture
    _cached_textures[path] = loaded_texture;

    // return texture
    return loaded_texture;

}

// Get relative crop rect of rect supposed to be rendered
SDL_Rect getIncludeCrop(SDL_Rect original_rect, SDL_Rect include_rect){

    // crop rect
    SDL_Rect crop_rect;

    // get intersection
    SDL_IntersectRect(&original_rect, &include_rect, &crop_rect);

    // fix crop rect
    crop_rect.x = crop_rect.x - original_rect.x;
    crop_rect.y = crop_rect.y - original_rect.y;
    crop_rect.h -= 2;

    // render rect (dont return this at the moment)
    // SDL_Rect rect = {
    //     int(original_rect.x + crop_rect.x),
    //     int(original_rect.y + 2 + crop_rect.y),
    //     int(crop_rect.w),
    //     int(crop_rect.h - 2),
    // };

    // return
    return crop_rect;

}

// Fill a circle. Code formatted from: https://gist.github.com/Gumichan01/332c26f6197a432db91cc4327fcabb1c
void SDL_RenderFillCircle(SDL_Renderer* renderer, int x, int y, int radius, CUI_Color color){

    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    int offsetx, offsety, d;
    int status;

    offsetx = 0;
    offsety = radius;
    d = radius -1;
    status = 0;

    while (offsety >= offsetx) {

        status += SDL_RenderDrawLine(renderer, x - offsety, y + offsetx, x + offsety, y + offsetx);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y + offsety, x + offsetx, y + offsety);
        status += SDL_RenderDrawLine(renderer, x - offsetx, y - offsety, x + offsetx, y - offsety);
        status += SDL_RenderDrawLine(renderer, x - offsety, y - offsetx, x + offsety, y - offsetx);

        if (status < 0) {
            status = -1;
            break;
        }

        if (d >= 2 * offsetx) {
            d -= 2 * offsetx + 1;
            offsetx +=1;
        }
        else if (d < 2 * (radius - offsety)) {
            d += 2 * offsety - 1;
            offsety -= 1;
        }
        else {
            d += 2 * (offsety - offsetx - 1);
            offsety -= 1;
            offsetx += 1;
        }
    }

}

// Render a rectangle with rounded edges.
void drawRoundedRect(SDL_Renderer* renderer, SDL_Rect original_rect, int radius, CUI_Color color){

    // set render draw color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // render inner rectangle
    SDL_Rect inner_rect = {
        original_rect.x + radius, original_rect.y + radius,
        original_rect.w - radius * 2, original_rect.h - radius * 2
    };
    SDL_RenderFillRect(renderer, &inner_rect);

    // circle dimensions
    int circle_x, circle_y;

    // render first circle
    circle_x = inner_rect.x;
    circle_y = inner_rect.y;
    SDL_RenderFillCircle(renderer, circle_x, circle_y, radius, color);

    // render second circle
    circle_x += inner_rect.w;
    SDL_RenderFillCircle(renderer, circle_x, circle_y, radius, color);

    // render third circle
    circle_y += inner_rect.h;
    SDL_RenderFillCircle(renderer, circle_x, circle_y, radius, color);

    // render fourth circle
    circle_x -= inner_rect.w;
    SDL_RenderFillCircle(renderer, circle_x, circle_y, radius, color);

    // outer rect
    SDL_Rect outer_rect = {inner_rect.x, original_rect.y, inner_rect.w, radius};

    // fill top rect
    SDL_RenderFillRect(renderer, &outer_rect);

    // fill bottom rect
    outer_rect.y += inner_rect.h + radius;
    outer_rect.h += 1;
    SDL_RenderFillRect(renderer, &outer_rect);

    // fill left rect
    outer_rect = {original_rect.x, inner_rect.y, radius, inner_rect.h};
    SDL_RenderFillRect(renderer, &outer_rect);

    // fill right rect
    outer_rect.x += inner_rect.w + radius;
    outer_rect.w += 1;
    SDL_RenderFillRect(renderer, &outer_rect);

}