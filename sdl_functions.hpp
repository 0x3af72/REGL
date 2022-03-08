#include <iostream>
#include <unordered_map>

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