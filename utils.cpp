#include <algorithm>
#include <iostream>
#include "utils.hpp"

using std::cerr, std::endl;


int nsUtils::help() {
    std::cout << "CursorMower v0.5.0 -> https://github.com/MikronT/CursorMower"
            << endl
            << endl << "Usage"
            << endl << "  cursorMower \"file\""
            << endl
            << endl << "Layout file syntax"
            << endl << "  Window configuration"
            << endl << "    > console_width={columns}"
            << endl << "    > console_height={lines}"
            << endl << "        Set command line window dimensions"
            << endl << "    > console_margin={lines}"
            << endl << "        Set window margins"
            << endl << "    > console_color={0-f} {000000-ffffff}"
            << endl << "        Remap any of 16 available colors"
            << endl << "  UI building"
            << endl << "    > cursor{1/2}={x} {y}"
            << endl << "        Set the point to move cursor 1 or 2 to"
            << endl << "    > cursor{1/2}_up[={lines}]"
            << endl << "    > cursor{1/2}_down[={lines}]"
            << endl << "    > cursor{1/2}_left[={columns}]"
            << endl << "    > cursor{1/2}_right[={columns}]"
            << endl << "        Move any cursor relatively"
            << endl << "    > clear[=screen]"
            << endl << "        Clear an area"
            << endl << "    > color[={0-f}{0-f}]"
            << endl << "        Change colors"
            << endl << "    > text={literally any text}"
            << endl << "        Set what should be printed"
            << endl
            << endl << "Returns"
            << endl << "  0 | Everything is OK"
            << endl << "  " << ERROR_ARGS_COUNT << " | Not enough/too many arguments (no file specified)"
            << endl << "  " << ERROR_FILE << " | Error reading a file (file not found or not accessible)"
            << endl << "  " << ERROR_SYNTAX << " | Illegal line syntax (check /help)"
            << endl << "  " << ERROR_OUT_OF_BOUNDS << " | Out of screen buffer bounds (text or coords exceed window frame dimensions)"
            << endl << "  5 | Help message is shown"
            << endl;
    return 5;
}
void nsUtils::error(const int error, const string& msg) {
    switch (error) {
    case ERROR_ARGS_COUNT:
        cerr << "Wrong number of arguments: " << msg << endl;
        break;
    case ERROR_FILE:
        cerr << "Error reading the file " << msg << endl;
        break;
    case ERROR_SYNTAX:
        cerr << "Illegal formatting at line " << msg << endl;
        break;
    case ERROR_OUT_OF_BOUNDS:
        cerr << "Argument out of bounds: " << msg << endl;
        break;
    }
    std::quick_exit(error);
}

short nsUtils::to_short(const int number) { return static_cast<short>(number); }
short nsUtils::to_short(const string& text, const int fromLine) {
    if (!std::ranges::all_of(text, isdigit))
        error(ERROR_SYNTAX, std::to_string(fromLine));

    return static_cast<short>(stoi(text));
}

void nsUtils::normallizeCoords(const COORD& dims, COORD& point1, COORD& point2) {
    if (point1.X < 1)
        point1.X = 1;
    if (point1.Y < 1)
        point1.Y = 1;

    if (point2.X < 1)
        point2.X = 1;
    if (point2.Y < 1)
        point2.Y = 1;

    if (point1.X > dims.X)
        point1.X = dims.X;
    if (point1.Y > dims.Y)
        point1.Y = dims.Y;

    if (point2.X > dims.X)
        point2.X = dims.X;
    if (point2.Y > dims.Y)
        point2.Y = dims.Y;
}
void nsUtils::rearrangeCoords(COORD& point1, COORD& point2) {
    if (point1.X <= point2.X) {
        if (point1.Y > point2.Y) {
            //Swap by OY
            const auto cursor1_y = point1.Y;
            point1.Y = point2.Y;
            point2.Y = cursor1_y;
        }
    }
    else {
        if (point1.Y <= point2.Y) {
            //Swap by OX
            const auto cursor1_x = point1.X;
            point1.X = point2.X;
            point2.X = cursor1_x;
        }
        else {
            //Swap places
            const COORD cursor1_temp = point1;
            point1 = point2;
            point2 = cursor1_temp;
        }
    }
}
