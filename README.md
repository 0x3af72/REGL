# CUI
A lightweight GUI library in C++.

## COMPILING
To use CUI, you'll have to link some SDL2 binaries. A sample compile command would be:

`g++ main.cpp -I\"SDL2/include\" -L\"SDL2/lib\" -L\"SDL2_image/lib\" -L\"SDL2_ttf/lib\" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o main`

(please modify this command if your main.cpp is not in the same folder as cui.hpp)

## SETUP
A minimal CUI program:

```cpp
#include <iostream>
#include "cui.hpp"

int main(int argc, char* argv[]){

  cuiInit(); // initialize cui and sdl2
  
  bool running = true; // keep the program running in a while loop
  while (running){
    running = cuiUpdate(); // update the program
  }
  
  cuiQuit(); // close the program properly
  
  return 0;
}
```

## cuiInit
This function initializes SDL.
It sets the SDL window to fullscreen and makes it transparent.

```cpp
void cuiInit(bool create_exit_window = true)
```

`create_exit_window`: If this is set to `true`, an exit window is created at the top left of the desktop which the user can use to close the program.

## cuiUpdate
Call this function in a while loop.
It returns a `bool` which specifies whether the program has ended.
This function updates and renders child objects and handles events.

```cpp
bool cuiUpdate()
```

## cuiQuit
Call this function at the end of your program.
This function closes SDL.

```cpp
void cuiQuit()
```

## CUI_Object, CUI_ChildObject
CUI_Objects contain: `CUI_Window`

CUI_ChildObjects contain: `CUI_Text`, `CUI_Button`, `CUI_Checkbox`

Setting the `enabled` attribute of these objects to false will cause them to be ignored and not be rendered.

Child objects that are disabled will not take up space in windows.

## CUI_Window
Windows in CUI contain ui entities, known as child objects.
Windows can be moved around by holding on their title bar and dragging them around.

```cpp
CUI_Window* createWindow(
    std::string name,
    int x, int y,
    int width, int height,
    CUI_Color color, CUI_Color bar_color, std::string bar_text_color
)
```

`name`: The window title. This is displayed on top of the window.

`x`, `y`: Original position of the window. These values will change as the user is able to drag them around.

`width`, `height`: Window dimensions.

`color`: Background color of the window.

`bar_color`: Window bar color, where the title is displayed.

`bar_text_color`: Window title text color.

## CUI_Text
Texts in CUI display text in a parent window.

```cpp
CUI_Text* addText(
    CUI_Window* window,
    std::string text_content,
    float size,
    CUI_Color color,
    int nextline, int indent
)
```

`window`: The parent window.

`text_content`: Text to be displayed.

`size`: Size of text.

`color`: Color of text.

`nextline`: Number of pixels to render next child object by.

`indent`: Indentation of child object in pixels.

## CUI_Button
Buttons in CUI call a `void` function when clicked.

```cpp
CUI_Button* addButton(
    CUI_Window* window,
    std::string text, CUI_Color text_color, float text_size,
    std::function<void()> on_click,
    int width, int height, int nextline, int indent, int edge_radius,
    CUI_Color color, CUI_Color hovered_color, CUI_Color pressed_color
)
```

`window`: The parent window.

`text`: Button text.

`text_color`: Button text color.

`text_size`: Button text size.

`on_click`: Function to be called when this button is clicked.

`width`, `height`: Button dimensions.

`nextline`: Number of pixels to render next child object by.

`indent`: Indentation of child object in pixels.

`edge_radius`: Radius of edge. This changes how round the button is.

`color`: Original color of button.

`hovered_color`: Color of button when hovered.

`pressed_color`: Color of button when pressed.


## CUI_Checkbox
When clicked, checkboxes change a boolean value and call a `void` function.

```cpp
CUI_Checkbox* addCheckbox(
    CUI_Window* window,
    bool& change_bool,
    std::function<void()> on_click,
    int width, int nextline, int indent, int edge_radius,
    CUI_Color color, CUI_Color checked_color
)
```

`window`: The parent window.

`change_bool`: The boolean to change when the checkbox is clicked.

`on_click`: Function to be called when this checkbox is clicked.

`width`: Checkbox dimensions. Height is not an argument as checkboxes are squares.

`nextline`: Number of pixels to render next child object by.

`indent`: Indentation of child object in pixels.

`edge_radius`: Radius of edge. This changes how round the checkbox is.

`color`: Original color of checkbox.

`checked_color`: Color of checkbox when it is checked.
