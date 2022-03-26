# REGL
A lightweight GUI library in C++.

## COMPILING
To use REGL, you'll have to link some SDL2 binaries. A sample compile command would be:

`g++ main.cpp -I\"SDL2/include\" -L\"SDL2/lib\" -L\"SDL2_image/lib\" -L\"SDL2_ttf/lib\" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o main`

(please modify this command if your main.cpp is not in the same folder as regl.hpp)

## SETUP
A minimal REGL program:

```cpp
#include <iostream>
#include "regl.hpp"

int main(int argc, char* argv[]){

  reglInit(); // initialize regl and sdl2
  
  bool running = true; // keep the program running in a while loop
  while (running){
    running = reglUpdate(); // update the program
  }
  
  reglQuit(); // close the program properly
  
  return 0;
}
```

## reglInit
This function **initializes SDL**.

It sets the SDL window to fullscreen and makes it transparent.

```cpp
void reglInit(bool create_exit_window = true, Uint32 renderer_flags = REGL_RENDER_SOFTWARE)
```

`create_exit_window`: If this is set to `true`, an exit window is created at the top left of the desktop which the user can use to close the program.

`renderer_flags`: REGL renderer flags OR'd together.

## reglUpdate
Call this function in a while loop.

It returns a `bool` which specifies whether the program has ended.

This function **updates and renders** child objects and **handles events**.

```cpp
bool reglUpdate()
```

## reglQuit
Call this function at the end of your program.
This function closes SDL.

```cpp
void reglQuit()
```

## REGL defaults
There are some **default variables** in REGL.

**RENDERER FLAGS**

`REGL_RENDERER_GPU`: Use GPU **rendering**.
`REGL_RENDERER_SOFTWARE`: Use **software rendering**.

**REGL_NEXTLINE_DEFAULT**

Use this as the `nextline` argument when creating child objects for the nextline to be **automatically** set.

**REGL_SCENE_DEFAULT**

Use this as the `scene` argument when creating child objects for the window scene to be set to the **default scene**.

**REGL_COLOR**

You can access builtin colors using `REGL_COLOR_<color_name>`.

## REGL_Color
You can create a `REGL_Color` object in the format:

`REGL_Color(color_r, color_g, color_b, color_a)`

`color_a` is optional and set to `255` by default.

## REGL_Object, REGL_ChildObject
REGL_Objects contain: `REGL_Window`

REGL_ChildObjects contain: `REGL_Text`, `REGL_Button`, `REGL_Checkbox`

Setting the `enabled` attribute of these objects to false will cause them to be ignored and not be rendered.

Child objects that are disabled will not take up space in windows.

## REGL_Window
Windows in REGL contain **ui entities**, known as child objects.

Windows can be moved around by holding on their **title bar** and dragging them around.

**Scenes** in windows can be switched around by modifying the window's `current_scene` attribute.

```cpp
REGL_Window* createWindow(
    std::string name,
    int x, int y,
    int width, int height,
    REGL_Color color, REGL_Color bar_color, REGL_Color bar_text_color,
    int bar_height, int bar_text_y_offset, float bar_text_size,
    int scrollbar_width, REGL_Color scrollbar_color
)
```

`name`: The window title. This is displayed on top of the window.

`scene_name`: The scene to create this window in.

`x`, `y`: Original position of the window. These values will change as the user is able to drag them around.

`width`, `height`: Window dimensions.

`color`: Background color of the window.

`bar_color`: Window bar color, where the title is displayed.

`bar_text_color`: Window title text color.

`bar_height`: Window bar height.

`bar_text_y_offset`: The y-offset of the bar title text.

`bar_text_size`: The size of the window bar.

`scrollbar_width`: Scrollbar width.

`scrollbar_color`: Scrollbar color.

## REGL_Text
Texts in REGL **display text** in a parent window.

**Textwrap** can be applied to them for a more appealing look.

```cpp
REGL_Text* addText(
    REGL_Window* window, std::string scene_name,
    std::string text_content,
    bool wrapped, bool wrap_by_words, int wrap_margin_right,
    float size,
    REGL_Color color,
    int nextline, int indent
)
```

`window`: The parent window.

`scene_name`: The scene to create this text in.

`text_content`: Text to be displayed.

`wrapped`: Whether to apply textwrap.

`wrap_by_words`: Whether to wrap by words or just characters.

`wrap_margin_right`: Margin from the right of the window to wrap by.

`size`: Size of text.

`color`: Color of text.

`nextline`: Number of pixels to render next child object by.

`indent`: Indentation of child object in pixels.

## REGL_Button
Buttons in REGL call a `void` function when clicked.

```cpp
REGL_Button* addButton(
    REGL_Window* window, std::string scene_name,
    std::string text, REGL_Color text_color, float text_size,
    REGL_Color outline_color, float outline_width,
    std::function<void()> on_click,
    int width, int height, int nextline, int indent, int edge_radius,
    REGL_Color color, REGL_Color hovered_color, REGL_Color pressed_color
)
```

`window`: The parent window.

`scene_name`: The scene to create this button in.

`text`: Button text.

`text_color`: Button text color.

`text_size`: Button text size.

`outline_color`: Color of button outline.

`outline_width`: Width of button outline.

`on_click`: Function to be called when this button is clicked.

`width`, `height`: Button dimensions.

`nextline`: Number of pixels to render next child object by.

`indent`: Indentation of child object in pixels.

`edge_radius`: Radius of edge. This changes how round the button is.

`color`: Original color of button.

`hovered_color`: Color of button when hovered.

`pressed_color`: Color of button when pressed.


## REGL_Checkbox
When clicked, checkboxes **change a boolean value** and call a `void` function.

```cpp
REGL_Checkbox* addCheckbox(
    REGL_Window* window, std::string scene_name,
    bool& change_bool,
    std::function<void()> on_click,
    int width, int nextline, int indent, int edge_radius,
    REGL_Color outline_color, float outline_width,
    REGL_Color color, REGL_Color checked_color,
    REGL_Color tick_color
)
```

`window`: The parent window.

`scene_name`: The scene to create this checkbox in.

`change_bool`: The boolean to change when the checkbox is clicked.

`on_click`: Function to be called when this checkbox is clicked.

`width`: Checkbox dimensions. Height is not an argument as checkboxes are squares.

`nextline`: Number of pixels to render next child object by.

`indent`: Indentation of child object in pixels.

`edge_radius`: Radius of edge. This changes how round the checkbox is.

`outline_color`: Color of checkbox outline.

`outline_width`: Width of checkbox outline.

`color`: Original color of checkbox.

`checked_color`: Color of checkbox when it is checked.

`tick_color`: Color of the tick which is visible when the checkbox is checked.
