#include <algorithm>
#include <iostream>
#include "utils.hpp"

using namespace std;
using namespace nsUtils;


int nsUtils::help() {
    cout << "CursorMower Alpha v4.0 -> github.com/MikronT/CursorMower"
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
        << endl << "  " << ERROR_SYNTAX << " | Illegal line syntax (the syntax is wrong, check cursor /help)"
        << endl << "  " << ERROR_OUT_OF_BOUNDS << " | Out of screen buffer bounds (text or coords exceed window dims)"
        << endl << "  5 | Help message is shown"
        << endl;
    return 5;
}
void nsUtils::error(const int error, const string& msg) {
    switch (error) {
    case ERROR_ARGS_COUNT:
        cerr << "Wrong number of arguments: " << msg << endl;
        exit(error);
    case ERROR_FILE:
        cerr << "Error reading the file " << msg << endl;
        exit(error);
    case ERROR_SYNTAX:
        cerr << "Illegal formatting at line " << msg << endl;
        exit(error);
    case ERROR_OUT_OF_BOUNDS:
        cerr << "Argument out of bounds: " << msg << endl;
        exit(error);
    }
}

short nsUtils::to_short(const int number) { return static_cast<short>(number); }
short nsUtils::to_short(const string& text, const int fromLine) {
    if (!ranges::all_of(text, isdigit))
        error(ERROR_SYNTAX, to_string(fromLine));

    return static_cast<short>(stoi(text));
}

void nsUtils::rearrangeCoords(COORD& point1, COORD& point2) {
    if (point1.X <= point2.X) {
        if (point1.Y > point2.Y) {
            //Swap by OY
            const auto cursor1_y = point1.Y;
            point1.Y = point2.Y;
            point2.Y = cursor1_y;
        }
    } else {
        if (point1.Y <= point2.Y) {
            //Swap by OX
            const auto cursor1_x = point1.X;
            point1.X = point2.X;
            point2.X = cursor1_x;
        } else {
            //Swap places
            const COORD cursor1_temp = point1;
            point1 = point2;
            point2 = cursor1_temp;
        }
    }
}
