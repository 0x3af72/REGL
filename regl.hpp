#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>

#include "extern_functions.hpp"
#include "regl_objects.hpp"
#include "font_renderer.hpp"
#include "regl_defaults.hpp"
#include "color.hpp"

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"
#include "SDL2/include/SDL2/SDL_ttf.h"

#pragma once

// Function forward declarations.

// Create a REGL_Window.
REGL_Window* createWindow(
    std::string name,
    int x, int y,
    int width, int height,
    REGL_Color color, REGL_Color bar_color, REGL_Color bar_text_color,
    int bar_height, int bar_text_y_offset, float bar_text_size,
    int scrollbar_width, REGL_Color scrollbar_color
);

// Add REGL_Text to a window.
REGL_Text* addText(
    REGL_Window* window, std::string scene_name,
    std::string text_content,
    bool wrapped, bool wrap_by_words, int wrap_margin_left,
    float size,
    REGL_Color color,
    int nextline, int indent
);

// Add REGL_Button to a window.
REGL_Button* addButton(
    REGL_Window* window, std::string scene_name,
    std::string text, REGL_Color text_color, float text_size,
    REGL_Color outline_color, float outline_width,
    std::function<void()> on_click,
    int width, int height, int nextline, int indent, int edge_radius,
    REGL_Color color, REGL_Color hovered_color, REGL_Color pressed_color
);

// Add REGL_Checkbox to a window.
REGL_Checkbox* addCheckbox(
    REGL_Window* window, std::string scene_name,
    bool& change_bool,
    std::function<void()> on_click,
    int width, int nextline, int indent, int edge_radius,
    REGL_Color outline_color, float outline_width,
    REGL_Color color, REGL_Color checked_color,
    REGL_Color tick_color
);

// Close all SDL2 things.
void reglQuit();

// Exit window
REGL_Window* exit_window;

// SDL Window and Renderer.
SDL_Window* regl_window;
SDL_Renderer* regl_renderer;

// SDL Cursors.
std::unordered_map<std::string, SDL_Cursor*> regl_cursors;

// FPS Capping.
const int _FPS = 60;
const float _FRAME_DELAY = 1000 / _FPS;
Uint32 _framestart;
int _frametime;

// Update variables.
bool mouse_clicked, sent_mouse_clicked;
bool mouse_held, sent_mouse_held;
bool mouse_up, sent_mouse_up;
bool sent_hovered;
int scrolled;
bool sent_scrolled;
int top_index;

// Mouse position.
SDL_Rect mouse_rect;

// REGL Objects
std::vector<std::unique_ptr<REGL_Object>> ui_objects;

// Initialize SDL2.
void reglInit(bool create_exit_window = true, Uint32 renderer_flags = REGL_RENDER_SOFTWARE){

    // init sdl
    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG | IMG_INIT_TIF);
    TTF_Init();
    int window_width, window_height;
    getDesktopSize(window_width, window_height);
    regl_window = SDL_CreateWindow(
        "REGL Application",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        window_width, window_height,
        SDL_WINDOW_ALWAYS_ON_TOP | SDL_WINDOW_BORDERLESS
    );
    regl_renderer = SDL_CreateRenderer(regl_window, -1, renderer_flags);
    SDL_SetHint(SDL_HINT_MOUSE_FOCUS_CLICKTHROUGH, "1");

    // make window transparent
    makeWindowTransparent(regl_window, RGB(0, 0, 0));

    // initialize font
    loadFont(regl_renderer, "fonts/verdana.ttf");
    
    // create cursors
    regl_cursors["clickable"] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND);
    regl_cursors["text"] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_IBEAM);
    regl_cursors["default"] = SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_ARROW);

    // create exit window
    if (create_exit_window){
        exit_window = createWindow(
            "Exit program",
            0, 0,
            400, 100,
            REGL_Color(175, 225, 175), REGL_Color(9, 121, 105), REGL_COLOR_WHITE,
            30, 0, 0.8,
            10, REGL_COLOR_BLACK
        );
        addButton(
            exit_window, REGL_SCENE_DEFAULT,
            "EXIT",
            REGL_COLOR_BLACK, 1,
            REGL_COLOR_BLACK, 5,
            reglQuit,
            200, 70,
            70, 100,
            15,
            REGL_Color(95, 133, 117), REGL_Color(9, 121, 105), REGL_Color(79, 121, 66)
        );
    }

}

void reglQuit(){
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}

// Call this in your main loop.
bool reglUpdate(){

    // clear update variables
    mouse_clicked = false;
    mouse_held = false;
    mouse_up = false;
    sent_mouse_clicked = false;
    sent_mouse_held = false;
    sent_mouse_up = false;
    scrolled = 0;
    sent_scrolled = false;
    sent_hovered = false;

    // clear renderer
    SDL_SetRenderDrawColor(regl_renderer, 0, 0, 0, 0);
    SDL_RenderClear(regl_renderer);

    // reset cursor
    SDL_SetCursor(regl_cursors["default"]);

    // get mouse rect
    POINT mouse_pos;
    GetCursorPos(&mouse_pos);
    mouse_rect = {mouse_pos.x, mouse_pos.y, 1, 1};

    // check if mouse is held
    if (GetKeyState(VK_LBUTTON) < 0){
        mouse_held = true;
    }

    // handle events
    SDL_Event event;
    while (SDL_PollEvent(&event) != 0){;
        if (event.type == SDL_QUIT){
            return false;
        } else if (event.type == SDL_KEYDOWN){
            // SDL_Keycode key = event.key.keysym.sym;
        } else if (event.type == SDL_MOUSEBUTTONDOWN){ // mouse clicked?
            mouse_clicked = true;
        } else if (event.type == SDL_MOUSEBUTTONUP){ // mouse up?
            mouse_up = true;
        } else if (event.type == SDL_MOUSEWHEEL){
            if (event.wheel.y > 0){
                scrolled = -1;
            } else if (event.wheel.y < 0){
                scrolled = 1;
            }
        }
    }

    // rotate positions for making windows on top because we can't rotate while iterating
    top_index = -1;
    
    // send events to objects and update and render
    for (std::unique_ptr<REGL_Object>& object: ui_objects){

        if (object->enabled){

            // send events if collided with mouse
            if (object->collides(mouse_rect)){

                // send hover
                if (!sent_hovered){
                    object->hovered(mouse_rect);
                    sent_hovered = true;
                }

                // send clicked
                if (mouse_clicked && !sent_mouse_clicked){
                    object->clicked(mouse_rect);
                    sent_mouse_clicked = true;
                }

                // send hold
                if (mouse_held && !sent_mouse_held){
                    object->mouseHeld(mouse_rect);
                    sent_mouse_held = true;
                }

                // send mouse up
                if (mouse_up && !sent_mouse_up){
                    object->mouseUp(mouse_rect);
                    sent_mouse_up = true;
                }

                // send scroll
                if (scrolled != 0 && !sent_scrolled){
                    object->scrolled(mouse_rect, scrolled);
                    sent_scrolled = true;
                }

            }
            
            object->update();

        }
    }

    // rotate if have to
    if (top_index != -1){
        moveToBack(ui_objects, top_index);
    }

    // render in reverse
    for (int index = ui_objects.size() - 1; index != -1; index --){
        if (ui_objects[index]->enabled){
            ui_objects[index]->render(regl_renderer);
        }
    }

    // present renderer
    SDL_RenderPresent(regl_renderer);

    // cap fps
    _frametime = SDL_GetTicks() - _framestart;
    if (_FRAME_DELAY > _frametime){
        SDL_Delay(_FRAME_DELAY - _frametime);
    }
    _framestart = SDL_GetTicks();
    // std::cout << _FRAME_DELAY - _frametime << "\n";

    return true;
}

REGL_Window* createWindow(
    std::string name,
    int x, int y,
    int width, int height,
    REGL_Color color, REGL_Color bar_color, REGL_Color bar_text_color,
    int bar_height, int bar_text_y_offset, float bar_text_size,
    int scrollbar_width, REGL_Color scrollbar_color
){
    auto window_ptr = std::make_unique<REGL_Window>(
        name,
        x, y,
        width, height,
        color, bar_color, bar_text_color,
        bar_height, bar_text_y_offset, bar_text_size,
        scrollbar_width, scrollbar_color
    );
    auto returned_ptr = window_ptr.get();
    ui_objects.push_back(std::move(window_ptr));
    return returned_ptr;
}

REGL_Text* addText(
    REGL_Window* window, std::string scene_name,
    std::string text_content,
    bool wrapped, bool wrap_by_words, int wrap_margin_left,
    float size,
    REGL_Color color,
    int nextline, int indent
){
    auto text_object_ptr = std::make_unique<REGL_Text>(
        text_content,
        wrapped, wrap_by_words, wrap_margin_left,
        size,
        color,
        nextline, indent
    );
    auto returned_ptr = text_object_ptr.get();
    window->scenes[scene_name].push_back(std::move(text_object_ptr));
    return returned_ptr;
}

REGL_Button* addButton(
    REGL_Window* window, std::string scene_name,
    std::string text, REGL_Color text_color, float text_size,
    REGL_Color outline_color, float outline_width,
    std::function<void()> on_click,
    int width, int height, int nextline, int indent, int edge_radius,
    REGL_Color color, REGL_Color hovered_color, REGL_Color pressed_color
){
    auto button_object_ptr = std::make_unique<REGL_Button>(
        text, text_color, text_size,
        outline_color, outline_width,
        on_click,
        width, height, nextline, indent, edge_radius,
        color, hovered_color, pressed_color
    );
    auto returned_ptr = button_object_ptr.get();
    window->scenes[scene_name].push_back(std::move(button_object_ptr));
    return returned_ptr;
}

REGL_Checkbox* addCheckbox(
    REGL_Window* window, std::string scene_name,
    bool& change_bool,
    std::function<void()> on_click,
    int width, int nextline, int indent, int edge_radius,
    REGL_Color outline_color, float outline_width,
    REGL_Color color, REGL_Color checked_color,
    REGL_Color tick_color
){
    auto checkbox_object_ptr = std::make_unique<REGL_Checkbox>(
        change_bool,
        on_click,
        width, nextline, indent, edge_radius,
        outline_color, outline_width,
        color, checked_color,
        tick_color
    );
    auto returned_ptr = checkbox_object_ptr.get();
    window->scenes[scene_name].push_back(std::move(checkbox_object_ptr));
    return returned_ptr;
}