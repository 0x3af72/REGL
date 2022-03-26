#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "extern_functions.hpp"
#include "sdl_functions.hpp"
#include "font_renderer.hpp"
#include "color.hpp"
#include "regl_defaults.hpp"

#include "SDL2/include/SDL2/SDL.h"
#include "SDL2/include/SDL2/SDL_image.h"

#pragma once

// Class forward declarations.
class REGL_Object;
class REGL_ChildObject;
class REGL_Window;
class REGL_Text;
class REGL_Button;

// Variables that REGL_Objects might need.
extern bool mouse_held;
extern bool mouse_clicked;
extern int top_index;
extern SDL_Rect mouse_rect;
extern std::unordered_map<std::string, SDL_Cursor*> regl_cursors;
extern std::vector<std::unique_ptr<REGL_Object>> ui_objects;

// Base class for all rendered objects.
class REGL_Object{

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
        virtual ~REGL_Object(){};

        // constructor
        REGL_Object(int x, int y, int width, int height, REGL_Color color);
        REGL_Object() = default;

};

REGL_Object::REGL_Object(int x, int y, int width, int height, REGL_Color color){

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

void REGL_Object::render(SDL_Renderer* renderer){

    // set render draw color to this object's color
    SDL_SetRenderDrawColor(renderer, color_r, color_g, color_b, color_a);

    // draw a rect according to this object's rect
    SDL_RenderFillRect(renderer, &rect);

}

void REGL_Object::update(){
    // update rect
    rect = {x, y, width, height};
}

bool REGL_Object::collides(SDL_Rect other_rect){
    return SDL_HasIntersection(&rect, &other_rect);
}

// Child object class. Just has a different render function.
class REGL_ChildObject{

    public:

        int nextline; // number of pixels to render next child
        int indent; // indentation in pixels
        bool enabled = true; // ignore this if it is disabled
        int decided_nextline; // use this if the nextline was selected as default

        // different render function from regl objects
        virtual void render(SDL_Renderer* renderer, int x, int y, REGL_Window* window){};

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

        // get nextline
        int getNextline(){return (nextline == REGL_NEXTLINE_DEFAULT) ? decided_nextline : nextline;};

        // input function
        void input(SDL_Keycode key);

        // destructor
        ~REGL_ChildObject(){};

        // constructor
        REGL_ChildObject(int nextline, int indent){this->nextline = nextline; this->indent = indent;};
        REGL_ChildObject() = default;

};

// Window class.
class REGL_Window : public REGL_Object{

    public:

        std::string name; // window name
        SDL_Rect bar_rect; // bar rect
        REGL_Color bar_color; // bar color
        REGL_Color bar_text_color; // bar text color
        int bar_text_y_offset; // bar text y offset
        float bar_text_size; // bar text size
        int bar_height; // bar text
        int scrollbar_width; // horizontal scrollbar width
        REGL_Color scrollbar_color; // scrollbar color
        SDL_Rect scrollbar_rect; // scrollbar rect
        bool scrollbar_held = false; // scrollbar held?
        int before_held_scroll_y; // scroll y before holding scrollbar
        bool SHOW_SCROLLBAR = true; // whether to show scrollbar
        bool is_held = false; // bool for checking if window is held
        int hold_origin_x, hold_origin_y; // hold origins
        int before_held_x, before_held_y; // before holding positions
        int mouse_before_held_x, mouse_before_held_y; // mouse hold origins
        bool minimized = false; // minimized bool
        int SCROLL_SCALE = 10; // scroll scale
        int viewport_y; // viewport ycoord
        int before_held_viewport_y; // viewport y before being held
        int before_held_viewport_difference; // viewport difference before being held
        int child_objects_height; // total child object height, calculated in render function
        std::unordered_map<std::string, std::vector<std::unique_ptr<REGL_ChildObject>>> scenes; // scenes
        std::string current_scene = REGL_SCENE_DEFAULT; // current scene
        bool ON_TOP_WHEN_CLICKED = true;

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
        bool collidedChild(SDL_Rect other_rect, REGL_ChildObject* &result_child);

        // move this window to the top
        void moveOnTop();

        // constructor
        REGL_Window(
            std::string name,
            int x, int y,
            int width, int height,
            REGL_Color color, REGL_Color bar_color, REGL_Color bar_text_color,
            int bar_height, int bar_text_y_offset, float bar_text_size,
            int scrollbar_width, REGL_Color scrollbar_color
        );

};

REGL_Window::REGL_Window(
    std::string name,
    int x, int y,
    int width, int height,
    REGL_Color color, REGL_Color bar_color, REGL_Color bar_text_color,
    int bar_height, int bar_text_y_offset, float bar_text_size,
    int scrollbar_width, REGL_Color scrollbar_color
) : REGL_Object(x, y, width, height, color){

    this->name = name;
    this->bar_color = bar_color;
    this->bar_text_color = bar_text_color;
    this->bar_height = bar_height;
    this->viewport_y = bar_height;
    this->bar_text_y_offset = bar_text_y_offset;
    this->bar_text_size = bar_text_size;
    this->scrollbar_width = scrollbar_width;
    this->scrollbar_color = scrollbar_color;

}

bool REGL_Window::collidedChild(SDL_Rect other_rect, REGL_ChildObject* &result_child){
    int render_y = y + viewport_y + 10;
    for (std::unique_ptr<REGL_ChildObject>& child_object: scenes[current_scene]){
        if (child_object->collides(other_rect, x + child_object->indent, render_y)){
            result_child = child_object.get();
            return true;
        }

        // increment render y
        render_y += child_object->getNextline();
    }
    return false;
}

void REGL_Window::moveOnTop(){
    if (ON_TOP_WHEN_CLICKED){
        top_index = std::find_if(
            ui_objects.begin(), ui_objects.end(),
            [this](std::unique_ptr<REGL_Object>& ptr){return ptr.get() == this;}
        ) - ui_objects.begin();
    }
}

void REGL_Window::update(){

    // check if holding
    if (is_held || scrollbar_held){

        // check if new hold origin was registered
        if (hold_origin_x == -5552){

            // before held positions
            before_held_x = x;
            before_held_y = y;
            before_held_scroll_y = scrollbar_rect.y;

            // hold origins
            hold_origin_x = mouse_rect.x;
            hold_origin_y = mouse_rect.y;

            // viewport y before held
            before_held_viewport_y = viewport_y;

            // viewport difference
            before_held_viewport_difference = float(mouse_rect.y - before_held_scroll_y) / height * -child_objects_height;

        }
        
        if (is_held){

            // change position based on mouse position
            x = before_held_x + mouse_rect.x - hold_origin_x;
            y = before_held_y + mouse_rect.y - hold_origin_y;

        } else if (scrollbar_held){

            // change viewport y based on mouse position
            viewport_y = (
                float(mouse_rect.y - before_held_scroll_y) / height * -child_objects_height
                - before_held_viewport_difference
                + before_held_viewport_y
            );
            
            // clamp viewport
            viewport_y = regl_max(regl_min(viewport_y, bar_height), -(child_objects_height - height));
        }

        // check if stopped holding
        if (!mouse_held && !mouse_clicked){
            is_held = false;
            scrollbar_held = false;
        }

    } else {

        // reset hold origins
        hold_origin_x = -5552;
        hold_origin_y = -5552;

    }

}

void REGL_Window::clicked(SDL_Rect mouse_rect){

    // move on top
    moveOnTop();

    // mouse origins
    mouse_before_held_x = mouse_rect.x;
    mouse_before_held_y = mouse_rect.y;

    // check if dragging bar rect
    if (SDL_HasIntersection(&bar_rect, &mouse_rect)){
        is_held = true;
        return;
    }

    // check if held the scrollbar
    if (SDL_HasIntersection(&scrollbar_rect, &mouse_rect)){
        scrollbar_held = true;
        return;
    }

    // send click to children
    REGL_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->clicked(mouse_rect);
    }

}

void REGL_Window::mouseHeld(SDL_Rect mouse_rect){

    // send hold to children
    REGL_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->mouseHeld(mouse_rect);
    }

}

void REGL_Window::mouseUp(SDL_Rect mouse_rect){

    // check if hit bar rect
    if (SDL_HasIntersection(&bar_rect, &mouse_rect) && (mouse_rect.x == mouse_before_held_x) && (mouse_rect.y == mouse_before_held_y)){
        minimized = (!minimized);
    }

    // send mouseup to children
    REGL_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->mouseUp(mouse_rect);
    }

}

void REGL_Window::scrolled(SDL_Rect mouse_rect, int scrolled){

    // window scrolling
    if (scrolled == -1){

        viewport_y += regl_min(SCROLL_SCALE, std::abs(bar_height - viewport_y));

    } else if (scrolled == 1){

        // change viewport y
        if (child_objects_height > height){
            viewport_y -= regl_min(SCROLL_SCALE, std::abs(child_objects_height - height + viewport_y));
        }
    }

    // send scroll to children
    REGL_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->scrolled(mouse_rect, scrolled);
    }

}

void REGL_Window::hovered(SDL_Rect mouse_rect){
    // send hovered to children
    REGL_ChildObject* affected_child;
    if (collidedChild(mouse_rect, affected_child)){
        affected_child->hovered(mouse_rect);
    }
}

void REGL_Window::render(SDL_Renderer* renderer){

    // update rects
    rect = {x, y, width, height + bar_height};
    bar_rect = {x, y, width, bar_height};

    // render window stuff if not minimized
    if (!minimized){

        // draw background rect
        drawRoundedRect(renderer, rect, 5, REGL_Color(color_r, color_g, color_b, color_a));

        // fix rect for collision
        rect.y += bar_height;
        rect.h -= bar_height;

    }

    int render_y = y + viewport_y + 10;
    child_objects_height = 0; // reset child objects height and recalculate
    for (std::unique_ptr<REGL_ChildObject>& child_object: scenes[current_scene]){
        
        // ignore if disabled
        if (child_object->enabled){

            // check if minimized
            if (!minimized){

                // render and increase render y
                child_object->render(renderer, x + child_object->indent, render_y, this);

            }

            // increase render y
            render_y += child_object->getNextline();

            // get total height of child objects
            child_objects_height += child_object->getNextline();

        }

    }

    // draw bar
    drawRoundedRect(renderer, bar_rect, 5, bar_color);

    // fill bottom of drawn bar so it doesnt look weird
    SDL_Rect fill_rect = {bar_rect.x, bar_rect.y + bar_rect.h - int(5 / 2), width, 5};
    SDL_SetRenderDrawColor(renderer, bar_color.r, bar_color.g, bar_color.b, 255);
    SDL_RenderFillRect(renderer, &fill_rect);

    // write window title
    float text_width = textWidth(name, bar_text_size, width * 0.9) / 2;
    float render_x = x + (width / 2) - text_width;
    renderText(renderer, bar_text_color, name, render_x, y + bar_text_y_offset, bar_text_size, width * 0.9, bar_rect);

    // display scrollbar
    if ((child_objects_height > height) && SHOW_SCROLLBAR && !minimized){
        
        // get height and y
        int scrollbar_height = regl_max((height - 4) * (float(height) / child_objects_height), 10);
        int scrollbar_y = y + bar_height + 4 + int(height * abs(float(viewport_y - bar_height) / child_objects_height));

        // fix scrollbar y
        if (scrollbar_y + scrollbar_height > y + height + bar_height){
            scrollbar_y = (y + height + bar_height) - scrollbar_height;
        }

        // get scrollbar rect
        scrollbar_rect = {
            x + width - scrollbar_width - 2, scrollbar_y,
            scrollbar_width, scrollbar_height
        };

        // render scrollbar
        drawRoundedHorizontalLine(renderer, scrollbar_rect, scrollbar_color);

    }

}

bool REGL_Window::collides(SDL_Rect other_rect){

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
class REGL_Text : public REGL_ChildObject{

    public:

        std::string text_content; // text content
        float size; // size of text
        REGL_Color color; // color
        bool wrapped; // whether to wrap text
        bool wrap_by_words; // whether to wrap by words
        int wrap_margin_left; // margin to wrap by
        int end_indents = 0; // number of newlines at the end of the text

        // custom render function
        void render(SDL_Renderer* renderer, int x, int y, REGL_Window* window) override;

        // custom collides functin
        bool collides(SDL_Rect other_rect){return true;};

        // change text
        void changeText(std::string new_text);

        // constructor
        REGL_Text(
            std::string text_content,
            bool wrapped, bool wrap_by_words, int wrap_margin_left,
            float size,
            REGL_Color color,
            int nextline, int indent);
        REGL_Text() = default;

};

REGL_Text::REGL_Text(
    std::string text_content,
    bool wrapped, bool wrap_by_words, int wrap_margin_left,
    float size,
    REGL_Color color,
    int nextline, int indent
)
    : REGL_ChildObject(nextline, indent){

    this->text_content = text_content;
    this->size = size;
    this->color = color;
    this->wrapped = wrapped;
    this->wrap_by_words = wrap_by_words;
    this->wrap_margin_left = wrap_margin_left;

    if (nextline == REGL_NEXTLINE_DEFAULT){
        decided_nextline = textHeight(text_content, size) + 10;
    }

    // check if text content ends with newline, is illegal
    while (this->text_content.substr(this->text_content.size() - 1, this->text_content.size()) == "\n"){
        this->text_content = this->text_content.substr(0, this->text_content.size() - 1);
        this->end_indents += 1;
    }

}

void REGL_Text::changeText(std::string new_text){

    // reset end indents
    end_indents = 0;

    // check if text content ends with newline, is illegal
    while (new_text.substr(new_text.size() - 1, new_text.size()) == "\n"){
        new_text = new_text.substr(0, new_text.size() - 1);
        end_indents += 1;
    }

    // set textcontent
    text_content = new_text;

}

void REGL_Text::render(SDL_Renderer* renderer, int x, int y, REGL_Window* window){

    // simply render if no wrap
    if (!wrapped){
        renderText(renderer, color, text_content, x, y, size, window->width, window->rect);
        return;
    }

    // declare stuff
    std::vector<std::string> texts_to_render;
    int new_slice_index = 0;

    while (true){

        // get wrap index
        int wrap_index = textWrapIndex(text_content.substr(new_slice_index, -1), size, window->width - wrap_margin_left, wrap_by_words);

        // exit if no more wrap
        if (wrap_index == -1){
            break;
        }

        // add new substring
        texts_to_render.push_back(text_content.substr(new_slice_index, wrap_index));

        // change slice index
        new_slice_index += wrap_index;

    }

    // add last substring
    texts_to_render.push_back(text_content.substr(new_slice_index, text_content.size()));

    // render texts
    int text_y = y;
    for (std::string rendered_text: texts_to_render){
        renderText(renderer, color, rendered_text, x, text_y, size, window->width, window->rect);
        text_y += textHeight(text_content, size);
    }

    // set decided nextline
    decided_nextline = text_y - y + 10 + (textHeight(text_content, size) * end_indents);

}

// Button class.
class REGL_Button : public REGL_ChildObject{

    public:

        std::string text; // display text
        REGL_Color text_color; // text color
        float text_size; // text size
        REGL_Color outline_color; // outline color
        float outline_width; // outline width
        int edge_radius; // edge radius
        int offset_x, offset_y; // offset when shrink button
        int width, height; // dimensions
        int current_width, current_height; // so we can shrink the button
        std::function<void()> on_click; // function called when clicked
        REGL_Color hovered_color; // color when hovered
        REGL_Color pressed_color; // color when pressed
        REGL_Color color; // normal color
        REGL_Color current_color; // current render color

        // custom render function
        void render(SDL_Renderer* renderer, int x, int y, REGL_Window* window) override;

        // custom hovered function
        void hovered(SDL_Rect mouse_rect) override;

        // custom clicked function
        void clicked(SDL_Rect mouse_rect) override;

        // custom mouseheld function
        void mouseHeld(SDL_Rect mouse_rect) override;

        // custom collides function
        bool collides(SDL_Rect other_rect, int x, int y) override;

        // constructor
        REGL_Button(
            std::string text,
            REGL_Color text_color, float text_size,
            REGL_Color outline_color, float outline_width,
            std::function<void()> on_click,
            int width, int height, int nextline, int indent, int edge_radius,
            REGL_Color color, REGL_Color hovered_color, REGL_Color pressed_color
        );

};

REGL_Button::REGL_Button(
    std::string text,
    REGL_Color text_color, float text_size,
    REGL_Color outline_color, float outline_width,
    std::function<void()> on_click,
    int width, int height, int nextline, int indent, int edge_radius,
    REGL_Color color, REGL_Color hovered_color, REGL_Color pressed_color
) : REGL_ChildObject(nextline, indent){
    
    this->text = text;
    this->text_color = text_color;
    this->text_size = text_size;
    this->outline_color = outline_color;
    this->outline_width = outline_width;
    this->edge_radius = edge_radius;
    this->on_click = on_click;
    this->width = width;
    this->height = height;
    this->color = color;
    this->hovered_color = hovered_color;
    this->pressed_color = pressed_color;

    if (nextline == REGL_NEXTLINE_DEFAULT){
        decided_nextline = height + 10;
    }

}

void REGL_Button::render(SDL_Renderer* renderer, int x, int y, REGL_Window* window){

    // apply offsets
    x += offset_x;
    y += offset_y;

    // do stuff with cropping rects here
    SDL_Rect to_draw_rect = {x, y, current_width, current_height};
    SDL_Rect cropped_rect = getIncludeCrop(to_draw_rect, window->rect);
    SDL_Rect draw_rect = {x + cropped_rect.x, y + cropped_rect.y, cropped_rect.w, cropped_rect.h}; // for text collision

    // draw outline
    SDL_Rect outline_draw_rect = {
        int(x - outline_width / 2), int(y - outline_width / 2),
        int(current_width + outline_width) + 1, int(current_height + outline_width) + 1
    };
    drawRoundedRect(renderer, outline_draw_rect, edge_radius, outline_color, window->rect);

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

void REGL_Button::hovered(SDL_Rect mouse_rect){

    // change color
    current_color = hovered_color;

    // change cursor
    SDL_SetCursor(regl_cursors["clickable"]);

}

void REGL_Button::clicked(SDL_Rect mouse_rect){
    on_click(); // call function
}

void REGL_Button::mouseHeld(SDL_Rect mouse_rect){

    // change color
    current_color = pressed_color;

    // shrink
    current_width = width * 0.85;
    current_height = height * 0.85;

    // change offsets
    offset_x = width * 0.15 * 0.5;
    offset_y = height * 0.15 * 0.5;

}

bool REGL_Button::collides(SDL_Rect other_rect, int x, int y){
    SDL_Rect collision_rect = {x, y, width, height};
    return SDL_HasIntersection(&collision_rect, &other_rect);
}

class REGL_Checkbox : public REGL_ChildObject{

    public:

        bool checked = false; // checked bool
        int edge_radius; // edge radius
        int width, height; // dimensions
        REGL_Color outline_color; // outline color
        float outline_width; // outline width
        bool& change_bool; // bool to set when clicked
        std::function<void()> on_click; // function called when clicked
        REGL_Color pressed_color; // color when pressed
        REGL_Color color; // normal color
        REGL_Color checked_color; // current render color
        REGL_Color tick_color; // tick color
        SDL_Texture* tick_texture; // tick texture

        // custom render function
        void render(SDL_Renderer* renderer, int x, int y, REGL_Window* window) override;

        // custom clicked function
        void clicked(SDL_Rect mouse_rect) override;

        // custom hovered function
        void hovered(SDL_Rect mouse_rect) override;

        // custom collides function
        bool collides(SDL_Rect other_rect, int x, int y) override;

        // constructor
        REGL_Checkbox(
            bool& change_bool,
            std::function<void()> on_click,
            int width, int nextline, int indent, int edge_radius,
            REGL_Color outline_color, float outline_width,
            REGL_Color color, REGL_Color checked_color,
            REGL_Color tick_color
        );

};

REGL_Checkbox::REGL_Checkbox(
    bool& change_bool,
    std::function<void()> on_click,
    int width, int nextline, int indent, int edge_radius,
    REGL_Color outline_color, float outline_width,
    REGL_Color color, REGL_Color checked_color,
    REGL_Color tick_color
) : REGL_ChildObject(nextline, indent), change_bool(change_bool){

    this->on_click = on_click;
    this->width = width;
    this->height = width;
    this->edge_radius = edge_radius;
    this->outline_color = outline_color;
    this->outline_width = outline_width;
    this->color = color;
    this->checked_color = checked_color;
    this->tick_color = tick_color;

    if (nextline == REGL_NEXTLINE_DEFAULT){
        decided_nextline = height + 10;
    }

}

void REGL_Checkbox::render(SDL_Renderer* renderer, int x, int y, REGL_Window* window){

    // load texture if not loaded
    if (!tick_texture){
        tick_texture = loadTexture(renderer, "images/checkbox-tick.png");
    }

    // draw outline
    SDL_Rect outline_draw_rect = {
        int(x - outline_width / 2), int(y - outline_width / 2),
        int(width + outline_width) + 1, int(height + outline_width) + 1
    };
    drawRoundedRect(renderer, outline_draw_rect, edge_radius, outline_color, window->rect);

    // rect to draw
    SDL_Rect to_draw_rect = {x, y, width, height};

    // draw rounded rect
    drawRoundedRect(renderer, to_draw_rect, edge_radius, checked ? checked_color : color, window->rect);

    // draw tick if checked
    if (checked){

        // get tick rect
        SDL_Rect tick_rect = {x, int(y + height * 0.1f), int(width * 0.9f * 1.15f), int(height * 0.9)};

        // crop tick rect
        SDL_Rect cropped_rect = getIncludeCrop(tick_rect, window->rect);

        // get new render rect
        SDL_Rect render_rect = {
            tick_rect.x + cropped_rect.x,
            tick_rect.y + cropped_rect.y,
            cropped_rect.w,
            cropped_rect.h,
        };

        // fix crop rect
        cropped_rect.x = 425 * cropped_rect.x / (width * 0.9f * 1.15f);
        cropped_rect.y = 368 * cropped_rect.y / (height * 0.9);
        cropped_rect.w = 425 * cropped_rect.w / (width * 0.9f * 1.15f);
        cropped_rect.h = 368 * cropped_rect.h / (height * 0.9);

        // render tick
        SDL_SetTextureColorMod(tick_texture, tick_color.r, tick_color.g, tick_color.b);
        SDL_RenderCopy(renderer, tick_texture, &cropped_rect, &render_rect);

    }

}

void REGL_Checkbox::clicked(SDL_Rect mouse_rect){

    // set checked
    checked = (!checked);

    // set bool
    change_bool = checked;

    // execute function
    on_click();

}

bool REGL_Checkbox::collides(SDL_Rect other_rect, int x, int y){
    SDL_Rect collision_rect = {x, y, width, height};
    return SDL_HasIntersection(&collision_rect, &other_rect);
}

void REGL_Checkbox::hovered(SDL_Rect mouse_rect){
    SDL_SetCursor(regl_cursors["clickable"]); // set cursor to clickable
}