# CUI
CUI is a lightweight GUI library in C++ and uses SDL2 for rendering.

To use CUI, you'll have to link some SDL2 binaries. A sample compile command would be:
`g++ main.cpp -I\"SDL2/include\" -L\"SDL2/lib\" -L\"SDL2_image/lib\" -L\"SDL2_ttf/lib\" -Wall -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -o main`
(please modify this command if your main.cpp is not in the same folder as cui.hpp)
