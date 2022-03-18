#include <iostream>
#include <unordered_map>

#include "color.hpp"
#include "cui_defaults.hpp"

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"

#pragma once

std::unordered_map<std::string, SDL_Texture*> _cached_textures;
SDL_Texture* _circle_texture;

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
SDL_Rect getIncludeCrop(SDL_Rect original_rect, SDL_Rect include_rect, bool debug = false){

    // crop rect
    SDL_Rect crop_rect;

    // get intersection
    SDL_IntersectRect(&original_rect, &include_rect, &crop_rect);

    // fix crop rect
    crop_rect.x -= original_rect.x;
    crop_rect.y -= original_rect.y;

    // return
    if (crop_rect.h > 0 && crop_rect.w > 0){
        return crop_rect;
    } else {
        SDL_Rect empty_rect = {0, 0, 0, 0};
        return empty_rect;
    }

}

// Fill cropped rect.
void SDL_FillIncludeRect(SDL_Renderer* renderer, SDL_Rect original_rect, SDL_Rect include_rect){
    SDL_Rect cropped_rect = getIncludeCrop(original_rect, include_rect);
    SDL_Rect adjusted_rect = {
        original_rect.x + cropped_rect.x, original_rect.y + cropped_rect.y,
        cropped_rect.w, cropped_rect.h
    };
    if (adjusted_rect.w && adjusted_rect.h){
        SDL_RenderFillRect(renderer, &adjusted_rect);
    }
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
void drawRoundedRect(SDL_Renderer* renderer, SDL_Rect original_rect, int radius, CUI_Color color, SDL_Rect include_rect = _FILL_RECT_ALL){

    // Create circle texture if not created
    if (!_circle_texture){
        _circle_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 101, 101);
        SDL_SetTextureBlendMode(_circle_texture, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer, _circle_texture);
        SDL_RenderFillCircle(renderer, 50, 50, 50, CUI_COLOR_WHITE);
        SDL_SetRenderTarget(renderer, NULL);
    }

    // set render draw color
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);

    // render inner rectangle
    SDL_Rect inner_rect = {
        original_rect.x + radius, original_rect.y + radius,
        original_rect.w - radius * 2, original_rect.h - radius * 2
    };
    SDL_FillIncludeRect(renderer, inner_rect, include_rect);

    // circle dimensions
    SDL_Rect circle_rect {0, 0, radius * 2, radius * 2};

    // set circle color
    SDL_SetTextureColorMod(_circle_texture, color.r, color.g, color.b);

    // declare cropped and render rects
    SDL_Rect cropped_rect, render_rect;

    // render first circle
    circle_rect.x = inner_rect.x - radius;
    circle_rect.y = inner_rect.y - radius;

    // get part to crop
    cropped_rect = getIncludeCrop(circle_rect, include_rect);

    // get new circle rect
    render_rect = {
        circle_rect.x + cropped_rect.x,
        circle_rect.y + cropped_rect.y,
        cropped_rect.w,
        cropped_rect.h,
    };

    // fix cropped rect
    cropped_rect.x = 101 * cropped_rect.x / (radius * 2);
    cropped_rect.y = 101 * cropped_rect.y / (radius * 2);
    cropped_rect.w = 101 * cropped_rect.w / (radius * 2);
    cropped_rect.h = 101 * cropped_rect.h / (radius * 2);

    SDL_RenderCopy(renderer, _circle_texture, &cropped_rect, &render_rect);

    // render second circle
    circle_rect.x += inner_rect.w;

    // get part to crop
    cropped_rect = getIncludeCrop(circle_rect, include_rect, true);

    // get new circle rect
    render_rect = {
        circle_rect.x + cropped_rect.x,
        circle_rect.y + cropped_rect.y,
        cropped_rect.w,
        cropped_rect.h,
    };

    // fix cropped rect
    cropped_rect.x = 101 * cropped_rect.x / (radius * 2);
    cropped_rect.y = 101 * cropped_rect.y / (radius * 2);
    cropped_rect.w = 101 * cropped_rect.w / (radius * 2);
    cropped_rect.h = 101 * cropped_rect.h / (radius * 2);
    SDL_RenderCopy(renderer, _circle_texture, &cropped_rect, &render_rect);

    // render third circle
    circle_rect.y += inner_rect.h + 2;

    // get part to crop
    cropped_rect = getIncludeCrop(circle_rect, include_rect);

    // get new circle rect
    render_rect = {
        circle_rect.x + cropped_rect.x,
        circle_rect.y + cropped_rect.y,
        cropped_rect.w,
        cropped_rect.h,
    };

    // fix cropped rect
    cropped_rect.x = 101 * cropped_rect.x / (radius * 2);
    cropped_rect.y = 101 * cropped_rect.y / (radius * 2);
    cropped_rect.w = 101 * cropped_rect.w / (radius * 2);
    cropped_rect.h = 101 * cropped_rect.h / (radius * 2);

    SDL_RenderCopy(renderer, _circle_texture, &cropped_rect, &render_rect);

    // render fourth circle
    circle_rect.x -= inner_rect.w;

    // get part to crop
    cropped_rect = getIncludeCrop(circle_rect, include_rect);

    // get new circle rect
    render_rect = {
        circle_rect.x + cropped_rect.x,
        circle_rect.y + cropped_rect.y,
        cropped_rect.w,
        cropped_rect.h,
    };

    // fix cropped rect
    cropped_rect.x = 101 * cropped_rect.x / (radius * 2);
    cropped_rect.y = 101 * cropped_rect.y / (radius * 2);
    cropped_rect.w = 101 * cropped_rect.w / (radius * 2);
    cropped_rect.h = 101 * cropped_rect.h / (radius * 2);

    SDL_RenderCopy(renderer, _circle_texture, &cropped_rect, &render_rect);

    // outer rect
    SDL_Rect outer_rect = {inner_rect.x, original_rect.y - 1, inner_rect.w, radius + 2};

    // fill top rect
    SDL_FillIncludeRect(renderer, outer_rect, include_rect);

    // fill bottom rect
    outer_rect.y += inner_rect.h + radius - 2;
    outer_rect.h += 3;
    SDL_FillIncludeRect(renderer, outer_rect, include_rect);

    // fill left rect
    outer_rect = {original_rect.x, inner_rect.y, radius, inner_rect.h};
    SDL_FillIncludeRect(renderer, outer_rect, include_rect);

    // fill right rect
    outer_rect.x += inner_rect.w + radius;
    outer_rect.w += 1;
    SDL_FillIncludeRect(renderer, outer_rect, include_rect);

}