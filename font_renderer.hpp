#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

#include "color.hpp"
#include "sdl_functions.hpp"

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"
#include "SDL2/include/SDL2/SDL_ttf.h"

#pragma once

std::string _default_chars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()";
TTF_Font* font;

// Character class for managing characters and their respective textures.
class Character{

    public:

        // dimensions
        float width;
        float height;

        // texture
        SDL_Texture* texture;
        
        // render function
        int render(SDL_Renderer* renderer, int x, int y, float size, SDL_Rect include_rect, CUI_Color color);

        // constructor
        Character(SDL_Renderer* renderer, SDL_Surface* character_surf);
        Character() = default;

};

Character::Character(SDL_Renderer* renderer, SDL_Surface* character_surf){

    // store dimensions
    width = character_surf->w / 20; // 1
    height = character_surf->h / 20; // 1

    // convert surface to texture
    texture = SDL_CreateTextureFromSurface(renderer, character_surf);

    // set blendmode
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    // free surface
    SDL_FreeSurface(character_surf);

}

int Character::render(SDL_Renderer* renderer, int x, int y, float size, SDL_Rect include_rect, CUI_Color color){

    // get original rect
    SDL_Rect original_rect = {x, y, int(width * size), int(height * size)};

    // calculate crop rect
    SDL_Rect crop_rect = getIncludeCrop(original_rect, include_rect);

    // fix crop rect
    crop_rect.w *= 20 / size;
    crop_rect.h *= 20 / size;
    crop_rect.x *= 20 / size;
    crop_rect.y *= 20 / size;

    // render rect
    SDL_Rect rect = {
        int(x + crop_rect.x / 20 * size),
        int(y + 2 + crop_rect.y / 20 * size),
        int(size * width * crop_rect.w / 20 / width),
        int(size * height * crop_rect.h / 20 / height),
    };

    // set color
    SDL_SetTextureColorMod(texture, color.r, color.g, color.b);

    // render
    SDL_RenderCopy(renderer, texture, &crop_rect, &rect);

    // return width
    return size * width;

}

// Map of colors and maps of characters and their corresponding textures.
std::unordered_map<std::string, Character> character_map;

// Load a font file and edit the character map accordingly.
void loadFont(
    SDL_Renderer* renderer,
    std::string font_file,
    std::string characters = _default_chars
){

    // load font
    font = TTF_OpenFont(font_file.c_str(), 500); // 30

    // render text in black
    SDL_Color color = {255, 255, 255};

    // store each character
    for (char& character: characters){
        
        std::string char_str(1, character);
        character_map[char_str] = Character(renderer, TTF_RenderText_Blended(font, char_str.c_str(), color));
        
    }
    
}

// Render text.
void renderText(
    SDL_Renderer* renderer,
    CUI_Color color,
    std::string text,
    int x, int y,
    float size, float max = 10000,
    SDL_Rect include_rect = _FILL_RECT_ALL
){

    int original_x = x;

    // render characters
    for (char& character: text){

        std::string char_str(1, character);
        x += character_map[char_str].render(renderer, x, y, size, include_rect, color);

        // check if more than max width?
        if (x - original_x >= max){
            return;
        }

    }

}

// Get width of text to be rendered.
float textWidth(std::string text, float size, float max = 10000){

    if (!text.size()){return 0;}

    float width = 0;

    // go through widths
    for (char& character: text){

        std::string char_str(1, character);
        width += character_map[char_str].width * size;

        // check if width exceeded max
        if (width >= max){
            return width;
        }

    }

    return width;

}

// Get height of text to be rendered.
float textHeight(std::string text, float size){

    if (!text.size()){return 0;}

    // all characters have the same height
    return character_map[text.substr(0, 1)].height * size;

}