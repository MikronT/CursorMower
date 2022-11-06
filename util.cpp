#include <algorithm>
#include <sstream>
#include "util.hpp"


int nsUtils::help() {
    wprintf(LR"(
CursorMower v0.5.4 -> https://github.com/MikronT/CursorMower

Usage
  cursorMower "file"

Layout file syntax
  Window configuration
    > console_width={columns}
    > console_height={lines}
        Set command line window dimensions
    > console_margin={lines}
        Set window margins
    > console_color={0-f}   {000000-ffffff}
    > console_color={color} {000000-ffffff}
        Remap any of 16 available colors
  UI building
    > cursor{1/2}={x} {y}
        Set the point to move cursor 1 or 2 to
    > cursor{1/2}_up[={lines}]
    > cursor{1/2}_down[={lines}]
    > cursor{1/2}_left[={columns}]
    > cursor{1/2}_right[={columns}]
        Move any cursor relatively
    > clear[=screen]
        Clear an area
    > color[={0-f}{0-f}]
        Change colors
    > text={literally any text}
        Set what should be printed

Error levels
  0 | Everything is OK
  %d | Not enough/too many arguments (no file specified)
  %d | Error reading a file(file not found or not accessible)
  %d | Illegal line syntax(check docs or use / help)
  %d | Out of screen buffer bounds(text or coords exceed window frame dimensions)
  5 | Help message is shown
)", ERROR_ARGS_COUNT, ERROR_FILE, ERROR_SYNTAX, ERROR_OUT_OF_BOUNDS);
    return 5;
}
void nsUtils::error(const int error, const wstring& msg) {
    switch (error) {
        case ERROR_ARGS_COUNT:
            wprintf(L"Wrong number of arguments: %s\n", msg.data());
            break;
        case ERROR_FILE:
            wprintf(L"Error reading the file\n");
            break;
        case ERROR_SYNTAX:
            wprintf(L"Illegal formatting at line %s\n", msg.data());
            break;
        case ERROR_OUT_OF_BOUNDS:
            wprintf(L"Argument out of bounds: %s\n", msg.data());
            break;
    }
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
