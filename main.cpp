#include <iostream>
#include <string>
#include <vector>

#include "cui.hpp"

int main(int argc, char* argv[]){

    cuiInit();

    CUI_Window* test_win = createWindow("helloworldASDASJDLKJASDLJLASKDJLKASJDLKAJSDJLASKDJLAJSDLJASLKDJALKSJDLKSJLDKJl", 500, 500, 700, 500, 255, 255, 254, 255);
    CUI_Text* hmm = addText(test_win, "black text", 1, CUI_COLOR_BLACK, 50);
    std::vector<CUI_Text*> texts = {
        addText(test_win, "helloworld", 3, CUI_COLOR_BLUE, 80),
    };

    CUI_Window* test_2 = createWindow("test window", 0, 0, 700, 500, 0, 0, 0, 255);
    CUI_Text* txt = addText(test_2, "hello", 1, CUI_COLOR_ORANGE, 50);
    CUI_Text* txt2 = addText(test_2, "verylongtexttest 1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20 21 22 23 24", 0.8, CUI_COLOR_ORANGE, 50);
    CUI_Text* txt3 = addText(test_2, "1", 3, CUI_COLOR_BLUE, 80);
    CUI_Text* txt4 = addText(test_2, "2", 3, CUI_COLOR_BLUE, 80);
    CUI_Text* txt5 = addText(test_2, "3", 3, CUI_COLOR_BLUE, 80);
    CUI_Text* txt6 = addText(test_2, "4", 3, CUI_COLOR_BLUE, 80);
    CUI_Text* txt7 = addText(test_2, "5", 3, CUI_COLOR_BLUE, 80);
    addText(test_2, "6", 3, CUI_COLOR_BLUE, 80);

    test_2->name = "please work";

    bool running = true;
    int ticks = 0;
    while (running){
        running = cuiUpdate();
        ticks += 1;
        txt->text_content = std::to_string(ticks);
        // texts[texts.size() - 1].text_content = std::to_string(ticks);
    }

    cuiQuit();

    return 0;
}