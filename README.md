# CUI
CUI is a lightweight GUI library in C++ which uses SDL2 for rendering.

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
