#include <algorithm>
#include <iostream>
#include <Windows.h>
#include "string.hpp"

using namespace std;
using namespace nsStringLib;


int help() {
    cout << "Cursor Mover v1.0 [MikronT]" << endl
        << endl
        << "Usage" << endl
        << "  cursor {x} {y} \"text\"" << endl
        << "  cursor {left&right margin} {top&bottom margin} {x} {y} \"text\"" << endl
        << endl
        << "Notes" << endl
        << "  - Coords are starting from (0,0) at the top left corner" << endl
        << "  - Margins are applied to all the coords" << endl
        << "    For example: 1st(0) line with top margin 2 will result in 3rd(2) line" << endl
        << endl
        << "Returns" << endl
        << "  0 if all is OK" << endl
        << "  1 if there are not enough or too many args" << endl
        << "  2 when illegal argument specified (text instead of number)" << endl
        << "  3 if the text is out of screen buffer bounds" << endl
        << "  4 if help is shown" << endl;
    return 4;
}


int main(const int arg_count, char** arg_list) {
    if (arg_count == 1 ||
        arg_count == 2 && string(arg_list[1]) == "/help")
        return help();

    if (arg_count != 4 && arg_count != 6) {
        cerr << "Wrong number of arguments: " << to_string(arg_count - 1) << endl;
        return 1;
    }

    for (int i = 1; i < arg_count - 2; ++i) {
        if (const auto temp_arg = string(arg_list[i]);
            !ranges::all_of(temp_arg, isdigit)) {
            cerr << "Illegal argument: " << temp_arg << endl;
            return 2;
        }
    }


    CONSOLE_SCREEN_BUFFER_INFO console_buffer;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &console_buffer);

    auto& [left, top, right, bottom] = console_buffer.srWindow;
    const int console_cols = right - left + 1,
              console_lines = bottom - top + 1;


    const short arg_margin_left = arg_count == 4 ? 0 : stoi(arg_list[1]),
                arg_margin_top = arg_count == 4 ? 0 : stoi(arg_list[2]),
                arg_x = stoi(arg_list[arg_count == 4 ? 1 : 3]),
                arg_y = stoi(arg_list[arg_count == 4 ? 2 : 4]);
    const string arg_text = arg_list[arg_count == 4 ? 3 : 5];

    const auto x = arg_margin_left + arg_x,
               y = arg_margin_top + arg_y;


    if (x < 0 || x >= console_cols ||
        y < 0 || y >= console_lines) {
        cerr << "Argument out of bounds: "
            << to_string(x) << ";" << to_string(y)
            << " with bounds "
            << to_string(console_cols) << ":" << to_string(console_lines) << endl;
        return 3;
    }

    if (x + string_getSize(arg_text) >= console_cols) {
        cerr << "Text out of bounds: "
            << to_string(x) << ";" << to_string(y)
            << " with bounds "
            << to_string(console_cols) << ":" << to_string(console_lines)
            << " with text \"" << arg_text << "\"" << endl;
        return 3;
    }

    SetConsoleCursorPosition(
        GetStdHandle(STD_OUTPUT_HANDLE),
        COORD{
            static_cast<short>(x),
            static_cast<short>(y)
        });

    cout << string_cut(arg_text, console_cols - arg_margin_left);
    return 0;
}
