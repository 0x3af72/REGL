#include <iostream>
#include <vector>
#include <algorithm>
#include <memory>
#include "wtypes.h"
#include "SDL2/include/SDL2/SDL_syswm.h"
#include "Windows.h"

#pragma once

#define regl_min(a, b) ((a < b) ? a : b)
#define regl_max(a, b) ((a > b) ? a : b)

// Check if item is in vector.
template <typename SomeClass>
bool inVector(std::vector<SomeClass> vec, SomeClass item){
    for (SomeClass itm: vec){
        if (itm == item){
            return true;
        }
    }
    return false;
}

// Change index of element in vector.
template <typename SomeClass>
void moveToBack(std::vector<SomeClass> &vec, size_t old_index){
    std::rotate(vec.begin(), vec.begin() + old_index, vec.end());
}

// Get index of item in vector.
template <typename SomeClass>
int getIndex(std::vector<SomeClass> vec, SomeClass item){
    int index = 0;
    for (SomeClass itm: vec){
        if (itm == item){
            return index;
        }
        index += 1;
    }
    return -1;
}

// Get desktop size.
void getDesktopSize(int& width, int& height){
   RECT desktop;
   const HWND hDesktop = GetDesktopWindow();
   GetWindowRect(hDesktop, &desktop);
   width = desktop.right;
   height = desktop.bottom;
}

// Make SDL window transparent.
bool makeWindowTransparent(SDL_Window* window, COLORREF color_key) {

    // get window handle
    SDL_SysWMinfo wm_info;
    SDL_VERSION(&wm_info.version);
    SDL_GetWindowWMInfo(window, &wm_info);
    HWND hWnd = wm_info.info.win.window;

    // make window type layered
    SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

    // set colorkey
    return SetLayeredWindowAttributes(hWnd, color_key, 0, LWA_COLORKEY);

}