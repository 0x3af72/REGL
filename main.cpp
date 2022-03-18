#include <iostream>
#include <string>
#include <vector>

#include "cui.hpp"

int main(int argc, char* argv[]){

    cuiInit(true);

    CUI_Window* test_2 = createWindow(
        "test window",
        0, 0,
        700, 500,
        CUI_COLOR_BLACK,
        CUI_COLOR_BLACK,
        CUI_COLOR_GREEN
    );
    CUI_Text* txt = addText(test_2, "hello", 1, CUI_COLOR_ORANGE, 50, 20);
    addText(test_2, "vppppptexttest 1 2 3 22 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 2 21 22 23 24", 2.2, CUI_COLOR_ORANGE, 50, 20);
    std::vector<CUI_Text*> texts_2;

    CUI_Window* test_win = createWindow(
        "helloworldASDASJDLKJASDLJLASKDJLKASJDLKAJSDJLASKDJLAJSDLJASLKDJALKSJDLKSJLDKJl",
        500, 500,
        700, 500,
        CUI_COLOR_WHITE,
        CUI_Color(200, 200, 200),
        CUI_COLOR_BLACK
    );
    int click_count = 0;
    addText(test_win, "black text", 1, CUI_COLOR_BLACK, 50, 10);
    std::vector<CUI_Text*> texts = {
        addText(test_win, "helloworld", 3, CUI_COLOR_BLUE, 80, 10),
    };

    bool selected = false;
    addCheckbox(test_2, selected, [](){}, 50, 60, 10, 20, CUI_COLOR_ORANGE, CUI_COLOR_RED);
    for (int i = 0; i != 100; i++){
        if (i == 6){
            addButton(
                test_2,
                "helloworld12345678910",
                CUI_COLOR_BLUE,
                1,
                [&click_count](){click_count += 1; std::cout << "click " << click_count << "\n";},
                200, 100, 110, 10, 20, CUI_Color(200, 200, 200), CUI_Color(100, 100, 100), CUI_Color(50, 50, 50)
            );

        }
        addText(test_2, std::to_string(i), 3, CUI_COLOR_BLUE, 80, 10);
    }

    test_2->name = "please work";

    bool running = true;
    int ticks = 0;
    while (running){
        running = cuiUpdate();
        ticks += 1;
        txt->text_content = std::to_string(ticks);
        // texts[texts.size() - 1].text_content = std::to_string(ticks);
        // std::cout << selected << "\n";
    }

    cuiQuit();

    return 0;
}