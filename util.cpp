#include <algorithm>
#include <iostream>
#include <sstream>
#include <Windows.h>
#include "util.hpp"


int nsUtils::help() {
    std::wcout << L"CursorMower v0.5.3 -> https://github.com/MikronT/CursorMower"
       << endl
       << endl << L"Usage"
       << endl << L"  cursorMower \"file\""
       << endl
       << endl << L"Layout file syntax"
       << endl << L"  Window configuration"
       << endl << L"    > console_width={columns}"
       << endl << L"    > console_height={lines}"
       << endl << L"        Set command line window dimensions"
       << endl << L"    > console_margin={lines}"
       << endl << L"        Set window margins"
       << endl << L"    > console_color={0-f}   {000000-ffffff}"
       << endl << L"    > console_color={color} {000000-ffffff}"
       << endl << L"        Remap any of 16 available colors"
       << endl << L"  UI building"
       << endl << L"    > cursor{1/2}={x} {y}"
       << endl << L"        Set the point to move cursor 1 or 2 to"
       << endl << L"    > cursor{1/2}_up[={lines}]"
       << endl << L"    > cursor{1/2}_down[={lines}]"
       << endl << L"    > cursor{1/2}_left[={columns}]"
       << endl << L"    > cursor{1/2}_right[={columns}]"
       << endl << L"        Move any cursor relatively"
       << endl << L"    > clear[=screen]"
       << endl << L"        Clear an area"
       << endl << L"    > color[={0-f}{0-f}]"
       << endl << L"        Change colors"
       << endl << L"    > text={literally any text}"
       << endl << L"        Set what should be printed"
       << endl
       << endl << L"Error levels"
       << endl << L"  0 | Everything is OK"
       << endl << L"  " << ERROR_ARGS_COUNT    << L" | Not enough/too many arguments (no file specified)"
       << endl << L"  " << ERROR_FILE          << L" | Error reading a file (file not found or not accessible)"
       << endl << L"  " << ERROR_SYNTAX        << L" | Illegal line syntax (check docs or use /help)"
       << endl << L"  " << ERROR_OUT_OF_BOUNDS << L" | Out of screen buffer bounds (text or coords exceed window frame dimensions)"
       << endl << L"  5 | Help message is shown"
       << endl;
    return 5;
}
void nsUtils::error(const int error, const wstring& msg) {
    switch (error) {
        case ERROR_ARGS_COUNT:    wcerr << L"Wrong number of arguments:"; break;
        case ERROR_FILE:          wcerr << L"Error reading the file";     break;
        case ERROR_SYNTAX:        wcerr << L"Illegal formatting at line"; break;
        case ERROR_OUT_OF_BOUNDS: wcerr << L"Argument out of bounds:";    break;
    }

    wcerr << L" " << msg << endl;
    std::quick_exit(error);
}

short nsUtils::to_short(const int number) { return static_cast<short>(number); }
short nsUtils::to_short(const wstring& text, const int fromLine) {
    if (!std::ranges::all_of(text, isdigit))
        error(ERROR_SYNTAX, std::to_wstring(fromLine));

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
