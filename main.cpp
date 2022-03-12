#include <iostream>
#include <string>
#include <vector>

#include "cui.hpp"

int main(int argc, char* argv[]){

    cuiInit();

    CUI_Window* test_2 = createWindow(
        "test window",
        0, 0,
        700, 500,
        CUI_COLOR_BLACK,
        CUI_COLOR_BLACK,
        CUI_COLORCODE_GREEN
    );
    CUI_Text* txt = addText(test_2, "hello", 1, CUI_COLORCODE_ORANGE, 50);
    CUI_Text* txt2 = addText(test_2, "verylongtexttest 1 2 3 22 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 2 21 22 23 24", 2.2, CUI_COLORCODE_ORANGE, 50);
    std::vector<CUI_Text*> texts_2;

    CUI_Window* test_win = createWindow(
        "helloworldASDASJDLKJASDLJLASKDJLKASJDLKAJSDJLASKDJLAJSDLJASLKDJALKSJDLKSJLDKJl",
        500, 500,
        700, 500,
        CUI_COLOR_WHITE,
        CUI_Color(200, 200, 200),
        CUI_COLORCODE_BLACK
    );
    CUI_Text* hmm = addText(test_win, "black text", 1, CUI_COLORCODE_BLACK, 50);
    std::vector<CUI_Text*> texts = {
        addText(test_win, "helloworld", 3, CUI_COLORCODE_BLUE, 80),
    };

    for (int i = 0; i != 100; i++){
        addText(test_2, std::to_string(i), 3, CUI_COLORCODE_BLUE, 80);
    }

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