#include <iostream>
#include <memory>
#include <functional>

#include "extern_functions.hpp"
#include "sdl_functions.hpp"
#include "font_renderer.hpp"

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"

#pragma once

// Class forward declarations.
class CUI_Object;
class CUI_ChildObject;
class CUI_Window;
class CUI_Text;

// Variables that CUI_Objects might need.
extern bool mouse_held;
extern bool mouse_clicked;
extern SDL_Rect mouse_rect;

// Color class. Used in cui.hpp.
class CUI_Color{
    public:
        int r, g, b, a;
        CUI_Color(int r, int g, int b, int a = 255){
            this->r = r;
            this->g = g;
            this->b = b;
            this->a = a;
            if (r == 255 && g == 255 && b == 255){
                this->b = 254;
            }
        };
        CUI_Color() = default;
};

// Base class for all rendered objects.
class CUI_Object{

    public:

        // positions
        int x, y;
        int width, height;

        // dont do anything to it if disabled
        bool enabled = true;

        // render info
        Uint16 color_r, color_g, color_b, color_a;

        // rect info
        SDL_Rect rect;

        // render function
        virtual void render(SDL_Renderer* renderer);

        // update function
        virtual void update();

        // click function
        virtual void clicked(SDL_Rect mouse_rect){};

        // mouse held function
        virtual void mouseHeld(SDL_Rect mouse_rect){};

        // mouse up function
        virtual void mouseUp(SDL_Rect mouse_rect){};

        // scrolled function
        virtual void scrolled(SDL_Rect mouse_rect, int scrolled){};

        // input function
        void input(SDL_Keycode key);

        // collide function
        virtual bool collides(SDL_Rect other_rect);

        // deconstructor
        virtual ~CUI_Object(){};

        // constructor
        CUI_Object(int x, int y, int width, int height, CUI_Color color);
        CUI_Object() = default;

};

CUI_Object::CUI_Object(int x, int y, int width, int height, CUI_Color color){

    // set dimension and positions
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;

    // set color values
    this->color_r = color.r;
    this->color_g = color.g;
    this->color_b = color.b;
    this->color_a = color.a;

}

void CUI_Object::render(SDL_Renderer* renderer){

    // set render draw color to this object's color
    SDL_SetRenderDrawColor(renderer, color_r, color_g, color_b, color_a);

    // draw a rect according to this object's rect
    SDL_RenderFillRect(renderer, &rect);

}

void CUI_Object::update(){
    // update rect
    rect = {x, y, width, height};
}

bool CUI_Object::collides(SDL_Rect other_rect){
    return SDL_HasIntersection(&rect, &other_rect);
}

// Child object class. Just has a different render function.
class CUI_ChildObject{

    public:

        int nextline; // number of pixels to render next child

        // different render function from cui objects
        virtual void render(SDL_Renderer* renderer, int x, int y, CUI_Window* window){};

        // click function
        virtual void clicked(SDL_Rect mouse_rect){};

        // mouse held function
        virtual void mouseHeld(SDL_Rect mouse_rect){};

        // mouse up function
        virtual void mouseUp(SDL_Rect mouse_rect){};

        // scrolled function
        virtual void scrolled(SDL_Rect mouse_rect, int scrolled){};

        // input function
        void input(SDL_Keycode key);

        // destructor
        ~CUI_ChildObject(){};

        // constructor
        CUI_ChildObject(int nextline){this->nextline = nextline;};
        CUI_ChildObject() = default;

};

// Window class.
class CUI_Window : public CUI_Object{

    public:

        std::string name; // window name
        SDL_Rect bar_rect; // bar rect
        CUI_Color bar_color; // bar color
        std::string bar_text_color; // bar text color
        bool is_held = false; // bool for checking if window is held
        int hold_origin_x, hold_origin_y; // hold origins
        int before_held_x, before_held_y; // before holding positions
        bool minimized = false; // minimized bool
        int SCROLL_SCALE = 10; // scroll scale
        int viewport_y = 35; // viewport ycoord
        int child_objects_height;
        std::vector<std::unique_ptr<CUI_ChildObject>> child_objects; // children objects in this window

        // custom render function
        void render(SDL_Renderer* renderer) override;

        // custom keypress function
        void clicked(SDL_Rect mouse_rect) override;

        // custom mouseheld function
        void mouseHeld(SDL_Rect mouse_rect) override;

        // custom mouseup function
        void mouseUp(SDL_Rect mouse_rect) override;

        // custom scroll function
        void scrolled(SDL_Rect mouse_rect, int scrolled) override;

        // custom collides function
        bool collides(SDL_Rect other_rect) override;

        // custom update function
        void update() override;

        // constructor
        CUI_Window(std::string name, int x, int y, int width, int height, CUI_Color color, CUI_Color bar_color, std::string bar_text_color);

};

CUI_Window::CUI_Window(std::string name, int x, int y, int width, int height, CUI_Color color, CUI_Color bar_color, std::string bar_text_color)
    : CUI_Object(x, y, width, height, color){

    this->name = name; // set name
    this->bar_color = bar_color; // set bar color
    this->bar_text_color = bar_text_color; // set bar text color

}

void CUI_Window::update(){

    // update rects
    rect = {x, y, width, height + 35};
    bar_rect = {x, y, width, 35};

    // check if holding
    if (is_held){

        // check if new hold origin was registered
        if (hold_origin_x == -5552){

            // before held positions
            before_held_x = x;
            before_held_y = y;

            // hold origins
            hold_origin_x = mouse_rect.x;
            hold_origin_y = mouse_rect.y;

        } else {

            // change position based on mouse position
            x = before_held_x + mouse_rect.x - hold_origin_x;
            y = before_held_y + mouse_rect.y - hold_origin_y;

        }

        // check if stopped holding
        if (!mouse_held && !mouse_clicked){
            is_held = false;
        }

    } else {

        // reset hold origins
        hold_origin_x = -5552;
        hold_origin_y = -5552;

    }

}

void CUI_Window::clicked(SDL_Rect mouse_rect){
    // check if dragging bar rect
    if (SDL_HasIntersection(&bar_rect, &mouse_rect)){
        is_held = true;
    }
}

void CUI_Window::mouseHeld(SDL_Rect mouse_rect){}

void CUI_Window::mouseUp(SDL_Rect mouse_rect){
    // check if hit bar rect
    if (SDL_HasIntersection(&bar_rect, &mouse_rect) && (x == before_held_x && y == before_held_y)){
        minimized = (!minimized);
    }
}

void CUI_Window::scrolled(SDL_Rect mouse_rect, int scrolled){
    if (scrolled == -1){
        viewport_y += cui_min(SCROLL_SCALE, std::abs(35 - viewport_y));
    } else if (scrolled == 1){

        // change viewport y
        if (child_objects_height > height){
            viewport_y -= cui_min(SCROLL_SCALE, std::abs(child_objects_height - height + viewport_y));
        }
    }
}

void CUI_Window::render(SDL_Renderer* renderer){

    // set render draw color to this object's color
    SDL_SetRenderDrawColor(renderer, color_r, color_g, color_b, color_a);

    // render window stuff if not minimized
    if (!minimized){

        // draw a rect according to this object's rect
        SDL_RenderFillRect(renderer, &rect);

    }

    int render_y = y + viewport_y;
    child_objects_height = 0; // reset child objects height and recalculate
    for (std::unique_ptr<CUI_ChildObject>& child_object: child_objects){

        // check if out of height
        if (!(render_y > y + height) && (render_y + child_object->nextline >= y) && !minimized){

            // render and increase render y
            child_object->render(renderer, x + 10, render_y, this);

        }

        // increase render y
        render_y += child_object->nextline;

        // get total height of child objects
        child_objects_height += child_object->nextline;

    }

    // bar color
    SDL_SetRenderDrawColor(renderer, bar_color.r, bar_color.g, bar_color.b, 255);

    // draw bar
    SDL_RenderFillRect(renderer, &bar_rect);

    // write window title
    float text_width = textWidth(name, bar_text_color, 0.7, width * 0.9) / 2;
    float render_x = x + (width / 2) - text_width;
    renderText(renderer, bar_text_color, name, render_x, y + 5, 0.7, width * 0.9);

}

bool CUI_Window::collides(SDL_Rect other_rect){

    // collision with bar rect
    if (SDL_HasIntersection(&bar_rect, &other_rect)){
        return true;
    }

    // collision with main window if not minimized
    if (!minimized && SDL_HasIntersection(&rect, &other_rect)){
        return true;
    }

    return false;

}

// Text class.
class CUI_Text : public CUI_ChildObject{

    public:

        std::string text_content; // text content
        float size; // size of text
        std::string color; // color

        // custom render function
        void render(SDL_Renderer* renderer, int x, int y, CUI_Window* window) override;

        // constructor
        CUI_Text(std::string text_content, float size, std::string color, int nextline);
        CUI_Text() = default;

};

CUI_Text::CUI_Text(std::string text_content, float size, std::string color, int nextline)
    : CUI_ChildObject(nextline){

    this->text_content = text_content;
    this->size = size;
    this->color = color;

}

void CUI_Text::render(SDL_Renderer* renderer, int x, int y, CUI_Window* window){
    renderText(renderer, color, text_content, x, y, size, window->width, window->rect);
}

// Button class.
class CUI_Button : public CUI_ChildObject{

    public:

        std::function<void()> on_click; // function called when clicked

};