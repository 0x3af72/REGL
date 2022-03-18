#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>

#include "extern_functions.hpp"
#include "sdl_functions.hpp"
#include "font_renderer.hpp"
#include "color.hpp"
#include "cui_defaults.hpp"

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"

#pragma once

// Class forward declarations.
class CUI_Object;
class CUI_ChildObject;
class CUI_Window;
class CUI_Text;
class CUI_Button;

// Variables that CUI_Objects might need.
extern bool mouse_held;
extern bool mouse_clicked;
extern SDL_Rect mouse_rect;
extern std::unordered_map<std::string, SDL_Cursor*> cui_cursors;

// Base class for all rendered objects.
class CUI_Object{

    public:

        // positions
        int x, y;

        // dimensions
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

        // hovered function
        virtual void hovered(SDL_Rect mouse_rect){};

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
        int indent; // indentation in pixels
        bool enabled = true; // ignore this if it is disabled

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

        // hovered function
        virtual void hovered(SDL_Rect mouse_rect){};

        // collide function
        virtual bool collides(SDL_Rect other_rect, int x, int y){return false;};

        // input function
        void input(SDL_Keycode key);

        // destructor
        ~CUI_ChildObject(){};

        // constructor
        CUI_ChildObject(int nextline, int indent){this->nextline = nextline; this->indent = indent;};
        CUI_ChildObject() = default;

};

// Window class.
class CUI_Window : public CUI_Object{

    public:

        std::string name; // window name
        SDL_Rect bar_rect; // bar rect
        CUI_Color bar_color; // bar color
        CUI_Color bar_text_color; // bar text color
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

        // custom hovered function
        void hovered(SDL_Rect mouse_rect) override;

        // custom collides function
        bool collides(SDL_Rect other_rect) override;

        // custom update function
        void update() override;

        // return a child obejct that collides with another rect
        bool collidedChild(SDL_Rect other_rect, CUI_ChildObject* &result_child);

        // constructor
        CUI_Window(
            std::string name,
            int x, int y,
            int width, int height,
            CUI_Color color, CUI_Color bar_color, CUI_Color bar_text_color
        );

};

CUI_Window::CUI_Window(
    std::string name,
    int x, int y,
    int width, int height,
    CUI_Color color, CUI_Color bar_color, CUI_Color bar_text_color
) : CUI_Object(x, y, width, height, color){

    this->name = name; // set name
    this->bar_color = bar_color; // set bar color
    this->bar_text_color = bar_text_color; // set bar text color

}

bool CUI_Window::collidedChild(SDL_Rect other_rect, CUI_ChildObject* &result_child){
    int render_y = y + viewport_y + 10;
    for (std::unique_ptr<CUI_ChildObject>& child_object: child_objects){
        if (child_object->collides(other_rect, x + child_object->indent, render_y)){
            result_child = child_object.get();
            return true;
        }

        // increment render y
        render_y += child_object->nextline;
    }
    return false;
}

void CUI_Window::update(){

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

    // send click to children
    CUI_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->clicked(mouse_rect);
    }

}

void CUI_Window::mouseHeld(SDL_Rect mouse_rect){
    // send hold to children
    CUI_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->mouseHeld(mouse_rect);
    }
}

void CUI_Window::mouseUp(SDL_Rect mouse_rect){

    // check if hit bar rect
    if (SDL_HasIntersection(&bar_rect, &mouse_rect) && (x == before_held_x && y == before_held_y)){
        minimized = (!minimized);
    }

    // send mouseup to children
    CUI_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->mouseUp(mouse_rect);
    }

}

void CUI_Window::scrolled(SDL_Rect mouse_rect, int scrolled){

    // window scrolling
    if (scrolled == -1){
        viewport_y += cui_min(SCROLL_SCALE, std::abs(35 - viewport_y));
    } else if (scrolled == 1){

        // change viewport y
        if (child_objects_height > height){
            viewport_y -= cui_min(SCROLL_SCALE, std::abs(child_objects_height - height + viewport_y));
        }
    }

    // send scroll to children
    CUI_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->scrolled(mouse_rect, scrolled);
    }

}

void CUI_Window::hovered(SDL_Rect mouse_rect){
    // send hovered to children
    CUI_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->hovered(mouse_rect);
    }
}

void CUI_Window::render(SDL_Renderer* renderer){

    // update rects
    rect = {x, y, width, height + 35};
    bar_rect = {x, y, width, 35};

    // render window stuff if not minimized
    if (!minimized){

        // draw background rect
        drawRoundedRect(renderer, rect, 15, CUI_Color(color_r, color_g, color_b, color_a));

        // fix rect for collision
        rect.y += 35;
        rect.h -= 35;

    }

    int render_y = y + viewport_y + 10;
    child_objects_height = 0; // reset child objects height and recalculate
    for (std::unique_ptr<CUI_ChildObject>& child_object: child_objects){
        
        // ignore if disabled
        if (child_object->enabled){

            // check if minimized
            if (!minimized){

                // render and increase render y
                child_object->render(renderer, x + child_object->indent, render_y, this);

            }

            // increase render y
            render_y += child_object->nextline;

            // get total height of child objects
            child_objects_height += child_object->nextline;

        }

    }

    // draw bar
    drawRoundedRect(renderer, bar_rect, 15, bar_color);

    // fill bottom of drawn bar so it doesnt look weird
    SDL_Rect fill_rect = {bar_rect.x, bar_rect.y + 20, width + 1, 17};
    SDL_SetRenderDrawColor(renderer, bar_color.r, bar_color.g, bar_color.b, 255);
    SDL_RenderFillRect(renderer, &fill_rect);

    // write window title
    float text_width = textWidth(name, 0.7, width * 0.9) / 2;
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
        CUI_Color color; // color

        // custom render function
        void render(SDL_Renderer* renderer, int x, int y, CUI_Window* window) override;

        // custom collides functin
        bool collides(SDL_Rect other_rect){return true;};

        // constructor
        CUI_Text(std::string text_content, float size, CUI_Color color, int nextline, int indent);
        CUI_Text() = default;

};

CUI_Text::CUI_Text(std::string text_content, float size, CUI_Color color, int nextline, int indent)
    : CUI_ChildObject(nextline, indent){

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

        std::string text; // display text
        CUI_Color text_color; // text color
        float text_size; // text size
        int edge_radius; // edge radius
        int offset_x, offset_y; // offset when shrink button
        int width, height; // dimensions
        int current_width, current_height; // so we can shrink the button
        std::function<void()> on_click; // function called when clicked
        CUI_Color hovered_color; // color when hovered
        CUI_Color pressed_color; // color when pressed
        CUI_Color color; // normal color
        CUI_Color current_color; // current render color

        // custom render function
        void render(SDL_Renderer* renderer, int x, int y, CUI_Window* window) override;

        // custom hovered function
        void hovered(SDL_Rect mouse_rect) override;

        // custom clicked function
        void clicked(SDL_Rect mouse_rect) override;

        // custom mouseheld function
        void mouseHeld(SDL_Rect mouse_rect) override;

        // custom collides function
        bool collides(SDL_Rect other_rect, int x, int y) override;

        // constructor
        CUI_Button(
            std::string text,
            CUI_Color text_color, float text_size,
            std::function<void()> on_click,
            int width, int height, int nextline, int indent, int edge_radius,
            CUI_Color color, CUI_Color hovered_color, CUI_Color pressed_color
        );

};

CUI_Button::CUI_Button(
    std::string text,
    CUI_Color text_color, float text_size,
    std::function<void()> on_click,
    int width, int height, int nextline, int indent, int edge_radius,
    CUI_Color color, CUI_Color hovered_color, CUI_Color pressed_color
) : CUI_ChildObject(nextline, indent){
    
    this->text = text;
    this->text_color = text_color;
    this->text_size = text_size;
    this->edge_radius = edge_radius;
    this->on_click = on_click;
    this->width = width;
    this->height = height;
    this->color = color;
    this->hovered_color = hovered_color;
    this->pressed_color = pressed_color;

}

void CUI_Button::render(SDL_Renderer* renderer, int x, int y, CUI_Window* window){

    // apply offsets
    x += offset_x;
    y += offset_y;

    // do stuff with cropping rects here
    SDL_Rect to_draw_rect = {x, y, current_width, current_height};
    SDL_Rect cropped_rect = getIncludeCrop(to_draw_rect, window->rect);
    SDL_Rect draw_rect = {x + cropped_rect.x, y + cropped_rect.y, cropped_rect.w, cropped_rect.h}; // for text collision

    // draw rounded rect
    drawRoundedRect(renderer, to_draw_rect, edge_radius, current_color, window->rect);

    // render text
    renderText(
        renderer,
        text_color, text,
        (x + current_width / 2) - textWidth(text, text_size) / 2, (y + current_height / 2) - textHeight(text, text_size) / 2,
        text_size,
        10000,
        draw_rect
    );

    // set current render color
    current_color = color;

    // clear offsets
    offset_x = 0;
    offset_y = 0;

    // clear widths
    current_width = width;
    current_height = height;
    
}

void CUI_Button::hovered(SDL_Rect mouse_rect){

    // change color
    current_color = hovered_color;

    // change cursor
    SDL_SetCursor(cui_cursors["clickable"]);

}

void CUI_Button::clicked(SDL_Rect mouse_rect){
    on_click(); // call function
}

void CUI_Button::mouseHeld(SDL_Rect mouse_rect){

    // change color
    current_color = pressed_color;

    // shrink
    current_width = width * 0.85;
    current_height = height * 0.85;

    // change offsets
    offset_x = width * 0.15 * 0.5;
    offset_y = height * 0.15 * 0.5;

}

bool CUI_Button::collides(SDL_Rect other_rect, int x, int y){
    SDL_Rect collision_rect = {x, y, width, height};
    return SDL_HasIntersection(&collision_rect, &other_rect);
}

class CUI_Checkbox : public CUI_ChildObject{

    public:

        bool checked = false; // checked bool
        int edge_radius; // edge radius
        int width, height; // dimensions
        bool& change_bool; // bool to set when clicked
        std::function<void()> on_click; // function called when clicked
        CUI_Color pressed_color; // color when pressed
        CUI_Color color; // normal color
        CUI_Color checked_color; // current render color
        SDL_Texture* tick_texture; // tick texture

        // custom render function
        void render(SDL_Renderer* renderer, int x, int y, CUI_Window* window) override;

        // custom clicked function
        void clicked(SDL_Rect mouse_rect) override;

        // custom hovered function
        void hovered(SDL_Rect mouse_rect) override;

        // custom collides function
        bool collides(SDL_Rect other_rect, int x, int y) override;

        // constructor
        CUI_Checkbox(
            bool& change_bool,
            std::function<void()> on_click,
            int width, int nextline, int indent, int edge_radius,
            CUI_Color color, CUI_Color checked_color
        );

};

CUI_Checkbox::CUI_Checkbox(
    bool& change_bool,
    std::function<void()> on_click,
    int width, int nextline, int indent, int edge_radius,
    CUI_Color color, CUI_Color checked_color
) : CUI_ChildObject(nextline, indent), change_bool(change_bool){

    this->on_click = on_click;
    this->width = width;
    this->height = width;
    this->edge_radius = edge_radius;
    this->color = color;
    this->checked_color = checked_color;

}

void CUI_Checkbox::render(SDL_Renderer* renderer, int x, int y, CUI_Window* window){

    // load texture if not loaded
    // if (!tick_texture){
    //     tick_texture = loadTexture(renderer, "images/clean.png");
    // }

    // rect to draw
    SDL_Rect to_draw_rect = {x, y, width, height};

    // draw rounded rect
    drawRoundedRect(renderer, to_draw_rect, edge_radius, checked ? checked_color : color, window->rect);

    // draw tick
    // SDL_RenderCopy(renderer, tick_texture, NULL, &draw_rect);

}

void CUI_Checkbox::clicked(SDL_Rect mouse_rect){

    // set checked
    checked = (!checked);

    // set bool
    change_bool = checked;

}

bool CUI_Checkbox::collides(SDL_Rect other_rect, int x, int y){
    SDL_Rect collision_rect = {x, y, width, height};
    return SDL_HasIntersection(&collision_rect, &other_rect);
}

void CUI_Checkbox::hovered(SDL_Rect mouse_rect){
    SDL_SetCursor(cui_cursors["clickable"]); // set cursor to clickable
}