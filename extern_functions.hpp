#include <iostream>
#include <vector>
#include "wtypes.h"
#include "SDL2/include/SDL2/SDL_syswm.h"
#include "Windows.h"

#pragma once

#define cui_min(a, b) (((a) < (b)) ? (a) : (b))

// Check if item is in vector.
template <typename SomeClass>
bool inVector(SomeClass item, std::vector<SomeClass> vec){
    for (SomeClass itm: vec){
        if (itm == item){
            return true;
        }
    }
    return false;
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

    // set colrokey
    return SetLayeredWindowAttributes(hWnd, color_key, 0, LWA_COLORKEY);

}