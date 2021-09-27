#include <algorithm>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include "commandLine.hpp"
#include "string.hpp"

using namespace std;
using namespace nsCommandLineLib;
using namespace nsStringLib;


const int
    ERROR_ARGS_COUNT = 1,
    ERROR_FILE = 2,
    ERROR_FORMAT = 3,
    ERROR_OUT_OF_BOUNDS = 4;


struct Block {
    COORD point;
    vector<string> strings;
};


int help() {
    cout << "Cursor Mower v2.1 [github.com/MikronT]"
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
        << endl << "  text`{x}`{y}`text"
        << endl << "    - lets you set what and where should be printed"
        << endl << "    - coords are starting from (0,0) at the top left corner"
        << endl << "    - margins are applied to all the coords automatically"
        << endl << "  end`{x}`{y}"
        << endl << "    - sets the point to move cursor to at the end of printing"
        << endl << "    - can be only one"
        << endl
        << endl << "Returns"
        << endl << "  0 - all is OK"
        << endl << "  " << ERROR_ARGS_COUNT << " - not enough or too many args"
        << endl << "  " << ERROR_FILE << " - error reading a file"
        << endl << "  " << ERROR_FORMAT << " - illegal line format"
        << endl << "  " << ERROR_OUT_OF_BOUNDS << " - the text is out of screen buffer bounds"
        << endl << "  5 - help is shown"
        << endl;
    return 5;
}

void error(const int error, const string& msg) {
    switch (error) {
    case ERROR_ARGS_COUNT:
        cerr << "Wrong number of arguments: " << msg << endl;
        exit(error);
    case ERROR_FILE:
        cerr << "Error reading the file " << msg << endl;
        exit(error);
    case ERROR_FORMAT:
        cerr << "Illegal formatting at line " << msg << endl;
        exit(error);
    case ERROR_OUT_OF_BOUNDS:
        cerr << "Argument out of bounds: " << msg << endl;
        exit(error);
    }
}


void checkNumber(const vector<string>& cells, const int line) {
    for (string cell : cells) if (!ranges::all_of(cell, isdigit)) error(ERROR_FORMAT, to_string(line));
}


int main(const int arg_count, char** arg_list) {
    if (arg_count == 1 ||
        arg_count == 2 && string(arg_list[1]) == "/help")
        return help();

    if (arg_count != 2) error(ERROR_ARGS_COUNT, to_string(arg_count - 1));


    const string arg_file = arg_list[1];
    COORD arg_dims = {0, 0},
          arg_margins = {0, 0},
          arg_end = {-1, -1};
    vector<Line> arg_lines;


    //Parse input from the file
    ifstream layout;
    layout.open(arg_file);
    if (!layout.is_open()) error(ERROR_FILE, arg_file);

    string sourceLine;
    int i = 0;
    while (getline(layout, sourceLine)) {
        i++;
        vector<string> cell = string_split(sourceLine, '`', 4);
        if (cell.empty()) continue;

        auto coords = COORD{
            static_cast<short>(stoi(cell.at(1))),
            static_cast<short>(stoi(cell.at(2)))
        };

        if (cell.at(0) == "dims" ||
            cell.at(0) == "margins" ||
            cell.at(0) == "goto") {
            if (cell.size() < 3) error(ERROR_FORMAT, to_string(i) + ": " + sourceLine);
            checkNumber({cell.at(1), cell.at(2)}, i);
        }

        if (cell.at(0) == "dims") arg_dims = coords;
        else if (cell.at(0) == "margins") arg_margins = coords;
        else if (cell.at(0) == "goto") arg_end = coords;
        else if (cell.at(0) == "text") {
            if (cell.size() < 2) error(ERROR_FORMAT, to_string(i) + ": " + sourceLine);
            arg_lines.emplace_back(Line{coords, cell.at(1)});
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
            coords.Y < 0 || coords.Y >= arg_dims.Y)
            error(ERROR_OUT_OF_BOUNDS,
                  to_string(coords.X) + ";" +
                  to_string(coords.Y) +
                  " with bounds of " +
                  to_string(arg_dims.X) + ":" +
                  to_string(arg_dims.Y));


        if (coords.X + string_getSize(text) >= arg_dims.X)
            error(ERROR_OUT_OF_BOUNDS,
                  to_string(coords.X) + ";"
                  + to_string(coords.Y)
                  + " with bounds of "
                  + to_string(arg_dims.X) + ":"
                  + to_string(arg_dims.Y)
                  + " with text \"" + text + "\"");

        SetConsoleCursorPosition(console_out, coords);
        cout << string_cut(text, arg_dims.X - coords.X);
    }


    //Move cursor to the end point
    if (arg_end.X != -1 &&
        arg_end.Y != -1) {
        arg_end = {
            static_cast<short>(arg_end.X + arg_margins.X),
            static_cast<short>(arg_end.Y + arg_margins.Y)
        };

        SetConsoleCursorPosition(console_out, arg_end);
    }
    return 0;
}
