#include <algorithm>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "commandLine.hpp"
#include "string.hpp"

using namespace std;
using namespace nsCommandLineLib;
using namespace nsStringLib;


struct Line {
    COORD point;
    string text;
};


int help() {
    cout << "Cursor Mower v2.0 [github.com/MikronT]"
        << endl
        << endl << "Usage"
        << endl << "  cursor \"file\""
        << endl
        << endl << "Layout file syntax"
        << endl << "  dims`{columns}`{lines}"
        << endl << "    - lets you set console window dimensions"
        << endl << "    - can be only one"
        << endl << "  margins`{left&right margin}`{top&bottom margin}"
        << endl << "    - to set window margins"
        << endl << "    - margins are applied via changing console window size"
        << endl << "    - can be only one"
        << endl << "  line`{x}`{y}`text"
        << endl << "    - lets you set what and where should be printed"
        << endl << "    - coords are starting from (0,0) at the top left corner"
        << endl << "    - margins are applied to all the coords automatically"
        << endl << "  final`{x}`{y}"
        << endl << "    - sets the point to move cursor to at the end of printing"
        << endl << "    - can be only one"
        << endl
        << endl << "Returns"
        << endl << "  0 - all is OK"
        << endl << "  1 - not enough or too many args"
        << endl << "  2 - illegal line format"
        << endl << "  3 - the text is out of screen buffer bounds"
        << endl << "  4 - error reading a file"
        << endl << "  5 - help is shown"
        << endl;
    return 5;
}


int main(const int arg_count, char** arg_list) {
    if (arg_count == 1 ||
        arg_count == 2 && string(arg_list[1]) == "/help")
        return help();

    if (arg_count != 2) {
        cerr << "Wrong number of arguments: " << to_string(arg_count - 1) << endl;
        return 1;
    }


    const string arg_file = arg_list[1];
    COORD arg_dims = {0, 0},
          arg_margins = {0, 0},
          arg_final = {-1, -1};
    vector<Line> arg_lines;


    //Parse input from the file
    ifstream layout;
    layout.open(arg_file);
    if (!layout.is_open()) {
        cerr << "Error reading the file " << arg_file << endl;
        return 4;
    }

    string sourceLine;
    int i = 0;
    while (getline(layout, sourceLine)) {
        i++;
        vector<string> cell = string_split(sourceLine, '`', 4);
        if (cell.empty()) continue;

        if (cell.size() < 3 ||
            !ranges::all_of(cell.at(1), isdigit) ||
            !ranges::all_of(cell.at(2), isdigit)) {
            cerr << "Illegal line format in line "
                << to_string(i) << ": " << sourceLine << endl;
            return 2;
        }

        auto coords = COORD{
            static_cast<short>(stoi(cell.at(1))),
            static_cast<short>(stoi(cell.at(2)))
        };

        if (cell.at(0) == "dims") arg_dims = coords;
        else if (cell.at(0) == "margins") arg_margins = coords;
        else if (cell.at(0) == "final") arg_final = coords;
        else if (cell.at(0) == "line") {
            if (cell.size() < 4) {
                cerr << "Illegal line format in line "
                    << to_string(i) << ": " << sourceLine << endl;
                return 2;
            }

            arg_lines.emplace_back(Line{coords, cell.at(3)});
        }
    }
    layout.close();


    //Get/set screen dimensions
    HANDLE console_out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFOEX console_info = getConInfo(console_out);

    if (arg_dims.X == 0 || arg_dims.Y == 0) {
        auto& [left, top, right, bottom] = console_info.srWindow;

        arg_dims.X = right - left + 1 - arg_margins.X * 2;
        arg_dims.Y = bottom - top + 1 - arg_margins.Y * 2;
    } else {
        arg_dims.X = arg_dims.X + arg_margins.X * 2;
        arg_dims.Y = arg_dims.Y + arg_margins.Y * 2;

        console_info.dwSize = arg_dims;
        console_info.srWindow = {0, 0, arg_dims.X, arg_dims.Y};
        setConInfo(console_out, console_info);

        console_info = getConInfo(console_out);
        console_info.srWindow = {0, 0, arg_dims.X, arg_dims.Y};
        setConInfo(console_out, console_info);
    }


    //Write text to screen
    for (auto& [coords, text] : arg_lines) {
        coords = {
            static_cast<short>(coords.X + arg_margins.X),
            static_cast<short>(coords.Y + arg_margins.Y)
        };

        if (coords.X < 0 || coords.X >= arg_dims.X ||
            coords.Y < 0 || coords.Y >= arg_dims.Y) {
            cerr << "Argument out of bounds: "
                << to_string(coords.X) << ";"
                << to_string(coords.Y)
                << " with bounds of "
                << to_string(arg_dims.X) << ":"
                << to_string(arg_dims.Y) << endl;
            return 3;
        }


        if (coords.X + string_getSize(text) >= arg_dims.X) {
            cerr << "Text out of bounds: "
                << to_string(coords.X) << ";"
                << to_string(coords.Y)
                << " with bounds of "
                << to_string(arg_dims.X) << ":"
                << to_string(arg_dims.Y)
                << " with text \"" << text << "\"" << endl;
            return 3;
        }

        SetConsoleCursorPosition(console_out, coords);
        cout << string_cut(text, arg_dims.X - coords.X);
    }


    //Move cursor to the final point
    if (arg_final.X != -1 &&
        arg_final.Y != -1) {
        arg_final = {
            static_cast<short>(arg_final.X + arg_margins.X),
            static_cast<short>(arg_final.Y + arg_margins.Y)
        };

        SetConsoleCursorPosition(console_out, arg_final);
    }
    return 0;
}
