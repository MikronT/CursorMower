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
    cout << "CursorMower v3.1 -> github.com/MikronT/CursorMower"
        << endl
        << endl << "Usage"
        << endl << "  cursor \"file\""
        << endl
        << endl
        << endl << "Layout file syntax"
        << endl
        << endl << "  Window configuration"
        << endl << "  > dims`{columns}`{lines}"
        << endl << "      To set command line window dimensions"
        << endl << "  > margins`{left&right margin}`{top&bottom margin}"
        << endl << "      To set window margins"
        << endl << "      Margins are applied via changing console window size"
        << endl
        << endl << "  UI building"
        << endl << "  > goto`{x}`{y}"
        << endl << "      To set the point to move cursor to"
        << endl << "      Margins are applied to all the coords automatically"
        << endl << "      Applies to all the commands below"
        << endl << "  > clear"
        << endl << "      To clear everything"
        << endl << "  > clear`{length}"
        << endl << "      To clear line with the specified length"
        << endl << "  > clear`{x}`{y}"
        << endl << "      To clear the whole block to the point specified"
        << endl << "  > text`{any text with spaces}"
        << endl << "      To set what should be printed"
        << endl
        << endl
        << endl << "Notes"
        << endl << "  Coords are starting from (0,0) at the top left corner"
        << endl
        << endl
        << endl << "Returns"
        << endl << "  0 | All is OK"
        << endl << "  " << ERROR_ARGS_COUNT << " | Not enough/too many arguments (no file specified)"
        << endl << "  " << ERROR_FILE << " | Error reading a file (file not found or is inaccessible)"
        << endl << "  " << ERROR_FORMAT << " | Illegal line syntax (the syntax is wrong, check cursor /help)"
        << endl << "  " << ERROR_OUT_OF_BOUNDS << " | Out of screen buffer bounds (text or coords exceed window dims)"
        << endl << "  5 | Help message is shown"
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


short toShort(const string& text, const int line) {
    if (!ranges::all_of(text, isdigit))
        error(ERROR_FORMAT, to_string(line));

    return static_cast<short>(stoi(text));
}


int main(const int arg_count, char** arg_list) {
    if (arg_count == 1 ||
        arg_count == 2 && string(arg_list[1]) == "/help")
        return help();

    if (arg_count != 2)
        error(ERROR_ARGS_COUNT, to_string(arg_count - 1));


    const string arg_file = arg_list[1];
    COORD arg_dims = {0, 0},
          arg_margins = {0, 0};
    vector<Block> arg_blocks;


    //Parse input from the file
    ifstream layout;
    layout.open(arg_file);
    if (!layout.is_open())
        error(ERROR_FILE, arg_file);

    {
        string line;
        int line_i = 0;
        auto line_coords = COORD{0, 0};

        while (getline(layout, line)) {
            line_i++;
            vector<string> cell = string_split(line, '`', 3);
            if (cell.empty())
                continue;

            if (cell.at(0) == "dims" ||
                cell.at(0) == "margins" ||
                cell.at(0) == "goto") {
                if (cell.size() < 3)
                    error(ERROR_FORMAT, to_string(line_i) + ": " + line);

                line_coords = COORD{
                    toShort(cell.at(1), line_i),
                    toShort(cell.at(2), line_i)
                };
            }

            if (cell.at(0) == "dims")
                arg_dims = line_coords;

            else if (cell.at(0) == "margins")
                arg_margins = line_coords;

            else if (cell.at(0) == "goto")
                arg_blocks.emplace_back(Block{line_coords, {}});

            else if (cell.at(0) == "clear") {
                auto lines =
                    cell.size() == 1 ?
                        arg_dims.Y :
                        cell.size() == 2 ?
                            1 :
                            stoi(cell.at(2)) - arg_blocks.at(arg_blocks.size() - 1).point.Y;
                auto length =
                    cell.size() == 1 ?
                        arg_dims.X :
                        toShort(cell.at(1), line_i);
                auto text = string(length, ' ');

                if (cell.size() == 1)
                    arg_blocks.emplace_back(Block{COORD{0, 0}, {}});

                for (int i = 0; i < lines; i++)
                    arg_blocks
                        .at(arg_blocks.size() - 1)
                        .strings
                        .emplace_back(text);
            } else if (cell.at(0) == "text") {
                if (cell.size() < 2)
                    error(ERROR_FORMAT, to_string(line_i) + ": " + line);

                arg_blocks
                    .at(arg_blocks.size() - 1)
                    .strings
                    .emplace_back(cell.at(1));
            }
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
    for (auto& [coords, strings] : arg_blocks) {
        coords = {
            static_cast<short>(coords.X + arg_margins.X),
            static_cast<short>(coords.Y + arg_margins.Y)
        };

        if (strings.empty())
            SetConsoleCursorPosition(console_out, coords);
        else
            for (auto& i : strings) {
                if (coords.X < 0 || coords.X >= arg_dims.X ||
                    coords.Y < 0 || coords.Y >= arg_dims.Y)
                    error(ERROR_OUT_OF_BOUNDS,
                          to_string(coords.X) + ";" +
                          to_string(coords.Y) +
                          " with bounds of " +
                          to_string(arg_dims.X) + ":" +
                          to_string(arg_dims.Y));

                SetConsoleCursorPosition(console_out, coords);

                cout << string_cut(i, arg_dims.X - coords.X);

                coords = {
                    coords.X,
                    static_cast<short>(coords.Y + 1)
                };
            }
    }
    return 0;
}
