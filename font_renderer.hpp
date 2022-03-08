#include <iostream>
#include <unordered_map>

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"
#include "SDL2/include/SDL2/SDL_ttf.h"

#pragma once

std::string _default_chars = " abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890!@#$%^&*()";

// Color class. Used in cui.hpp.
class CUI_Color{
    public:
        int r, g, b;
        CUI_Color(int r, int g, int b){this->r = r; this->g = g, this->b = b;};
};

// Character class for managing characters and their respective textures.
class Character{

    public:

        // dimensions
        float width;
        float height;

        // texture
        SDL_Texture* texture;
        
        // render function
        int render(SDL_Renderer* renderer, int x, int y, float size);

        // constructor
        Character(SDL_Renderer* renderer, SDL_Surface* character_surf);
        Character() = default;

};

Character::Character(SDL_Renderer* renderer, SDL_Surface* character_surf){

    // store dimensions
    width = character_surf->w / 20;
    height = character_surf->h / 20;

    // convert surface to texture
    texture = SDL_CreateTextureFromSurface(renderer, character_surf);

    // free surface
    SDL_FreeSurface(character_surf);

}

int Character::render(SDL_Renderer* renderer, int x, int y, float size){

    // new render rect
    SDL_Rect rect = {x, y, int(size * width), int(size * height)};

    // render
    SDL_RenderCopy(renderer, texture, NULL, &rect);

    // return width
    return size * width;

}

// Map of colors and maps of characters and their corresponding textures.
std::unordered_map<std::string, std::unordered_map<std::string, Character>> character_map;

// // Load a font file and edit the character map accordingly.
void loadFont(SDL_Renderer* renderer, std::string color_name, std::string font_file, Uint8 color_r, Uint8 color_g, Uint8 color_b, std::string characters = _default_chars){

    // load font
    TTF_Font* font = TTF_OpenFont(font_file.c_str(), 500);

    // convert rgb to sdl color
    SDL_Color color = {color_r, color_g, color_b};

    // store each character
    for (char& character: characters){
        
        std::string char_str(1, character);
        character_map[color_name][char_str] = Character(renderer, TTF_RenderText_Solid(font, char_str.c_str(), color));

    }
    
}

// Render text.
void renderText(SDL_Renderer* renderer, std::string color_name, std::string text, int x, int y, float size, float max = 10000){

    int original_x = x;

    // render characters
    for (char& character: text){

        std::string char_str(1, character);
        x += character_map[color_name][char_str].render(renderer, x, y, size);

        // check if more than max width?
        if (x - original_x >= max){
            return;
        }

    }

}

// Get width of text to be rendered.
float textWidth(std::string text, std::string color_name, float size, float max = 10000){

    float width = 0;

    // go through widths
    for (char& character: text){

        std::string char_str(1, character);
        width += character_map[color_name][char_str].width * size;

        // check if width exceeded max
        if (width >= max){
            return width;
        }

    }

    return width;

}