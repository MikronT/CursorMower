#include <algorithm>
#include <sstream>
#include "util.hpp"

using std::quick_exit, std::string, std::to_string, std::vector;


int util::help() {
    printf(R"(
CursorMower v0.5.6 -> https://github.com/MikronT/CursorMower

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
)", ERROR_TYPE::WRONG_ARGS_NUMBER,
           ERROR_TYPE::FILE_READ_ERROR,
           ERROR_TYPE::BAD_SYNTAX,
           ERROR_TYPE::ARG_OUT_OF_BOUNDS);
    return 5;
}
void util::error(const ERROR_TYPE error, const string& msg) {
    switch (error) {
        case ERROR_TYPE::WRONG_ARGS_NUMBER:
            printf("Wrong number of arguments: %s\n", msg.data());
            break;
        case ERROR_TYPE::FILE_READ_ERROR:
            printf("Error reading the file\n");
            break;
        case ERROR_TYPE::BAD_SYNTAX:
            printf("Illegal formatting at line %s\n", msg.data());
            break;
        case ERROR_TYPE::ARG_OUT_OF_BOUNDS:
            printf("Argument out of bounds: %s\n", msg.data());
            break;
    }
    quick_exit(static_cast<int>(error));
}

short util::to_short(const int number) { return static_cast<short>(number); }
short util::to_short(const string& text, const int fromLine) {
    if (!std::ranges::all_of(text, isdigit))
        error(ERROR_TYPE::BAD_SYNTAX, to_string(fromLine));

    return static_cast<short>(stoi(text));
}

short util::getCoordArgument(const vector<string>& cells, const int line_i) {
    if (cells.size() == 1)
        return 1;
    return to_short(cells.at(1), line_i);
}
void util::normallizeCoords(const COORD& dims, COORD& point1, COORD& point2) {
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
void util::rearrangeCoords(COORD& point1, COORD& point2) {
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
