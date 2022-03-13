# CUI
CUI: A lightweight GUI library in C++ which uses SDL2 for rendering.

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
  
  return 0;
}
```

## CUI_Window
Windows in CUI contain ui entities, known as child objects.

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
    std::string color,
    int nextline
)
```

`window`: The parent window.

`text_content`: Text to be displayed.

`size`: Size of text.

`color`: Color of text.

`nextline`: Number of pixels to render next child object by.

## CUI_Button
Buttons in CUI call a `void` function when clicked.

```cpp
CUI_Button* addButton(
    CUI_Window* window,
    std::string text, std::string text_color, float text_size,
    std::function<void()> on_click,
    int width, int height, int nextline,
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

`color`: Original color of button.

`hovered_color`: Color of button when hovered.

`pressed_color`: Color of button when pressed.
