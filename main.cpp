#include <iostream>
#include <string>
#include <vector>

#include "regl.hpp"

#include <iostream>
#include "regl.hpp"

int main(int argc, char* argv[]){

    reglInit(); // initialize regl and sdl2

    // create an information window
    REGL_Window* information_window = createWindow(
        "What is REGL?",
        500, 500, 500, 700,
        REGL_Color(175, 225, 175), REGL_Color(9, 121, 105), REGL_COLOR_WHITE,
        30, 0, 0.8,
        10, REGL_COLOR_WHITE
    );

    // add text
    addText(
        information_window, REGL_SCENE_DEFAULT,
        "REGL",
        false, false, 0,
        2,
        REGL_COLOR_BLACK, REGL_NEXTLINE_DEFAULT, 10
    );

    addText(
        information_window, REGL_SCENE_DEFAULT,
        "REGL is a lightweight GUI library in C++.\n\n"
        "An option to integrate REGL into normal SDL programs will be added in the future.\n\n"
        "REGL aims for: \n\n"
        " - Simplicity\n"
        " - Speed\n\n",
        true, true, 15,
        1,
        REGL_COLOR_BLACK, REGL_NEXTLINE_DEFAULT, 10
    );

    addButton(
        information_window, REGL_SCENE_DEFAULT,
        "Show me more!", REGL_COLOR_BLACK, 0.7,
        REGL_COLOR_BLACK, 5,
        [&information_window]{information_window->current_scene = "DEMO";},
        200, 70,
        100, 10,
        15,
        REGL_Color(95, 133, 117), REGL_Color(9, 121, 105), REGL_Color(79, 121, 66)
    );

    addButton(
        information_window, REGL_SCENE_DEFAULT,
        "What's coming next?", REGL_COLOR_BLACK, 0.7,
        REGL_COLOR_BLACK, 5,
        [&information_window]{information_window->current_scene = "COMING_NEXT";},
        200, 70,
        REGL_NEXTLINE_DEFAULT, 10,
        15,
        REGL_Color(95, 133, 117), REGL_Color(9, 121, 105), REGL_Color(79, 121, 66)
    );

    addButton(
        information_window, "DEMO",
        "Back", REGL_COLOR_BLACK, 0.7,
        REGL_COLOR_BLACK, 5,
        [&information_window]{information_window->current_scene = REGL_SCENE_DEFAULT;},
        200, 70,
        100, 10,
        15,
        REGL_Color(95, 133, 117), REGL_Color(9, 121, 105), REGL_Color(79, 121, 66)
    );

    addButton(
        information_window, "COMING_NEXT",
        "Back", REGL_COLOR_BLACK, 0.7,
        REGL_COLOR_BLACK, 5,
        [&information_window]{information_window->current_scene = REGL_SCENE_DEFAULT;},
        200, 70,
        100, 10,
        15,
        REGL_Color(95, 133, 117), REGL_Color(9, 121, 105), REGL_Color(79, 121, 66)
    );

    // create a testing window
    REGL_Window* testing_window = createWindow(
        "Testing window",
        1000, 0, 700, 500,
        REGL_Color(175, 225, 175), REGL_Color(9, 121, 105), REGL_COLOR_WHITE,
        30, 0, 0.8,
        10, REGL_COLOR_WHITE
    );

    bool temp = false;
    addCheckbox(
        testing_window, REGL_SCENE_DEFAULT,
        temp, []{},
        50,
        1000, 10, 10,
        REGL_COLOR_WHITE, 1,
        REGL_COLOR_GREEN, REGL_COLOR_BLUE,
        REGL_COLOR_BLACK
    );

    bool running = true; // keep the program running in a while loop
    while (running){
        running = reglUpdate(); // update the program
    }

    reglQuit(); // close the program properly

    return 0;
}