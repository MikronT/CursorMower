#include "util.hpp"

using std::quick_exit, std::string, std::to_string;


int util::help() {
    printf(R"(
CursorMower v0.6.0 -> https://github.com/MikronT/CursorMower

Usage
  cursorMower "file"

Layout file syntax
  Window configuration
    > console_width={columns}
    > console_height={lines}
        Set command line window dimensions
    > console_margin={lines}
        Set inner window margin
    > console_color={0-f}   {000000-ffffff}
    > console_color={color} {000000-ffffff}
        Remap any of 16 available colors
  UI building
    > cursor{1/2}={x} {y}
        Specify the point to move cursor 1 or 2 to
    > cursor{1/2}_up[={lines}]
    > cursor{1/2}_down[={lines}]
    > cursor{1/2}_left[={columns}]
    > cursor{1/2}_right[={columns}]
        Move any cursor relatively
    > color[={0-f}{0-f}]
        Change pen color
    > clear[=screen]
        Draw a rectangle or clear screen
    > text={literally any text}
        Set what to print
    > caret
        Where to put the caret for input

Error levels
  0 | Everything is fine
  %d | Wrong number of command line arguments (or file specified)
  %d | Error reading the file (file not found or not accessible)
  %d | Illegal line syntax (check docs or use /help)
  %d | Out of screen buffer bounds (text or coords exceed window frame dimensions)
  5 | Help message is shown
)",
           ERROR_TYPE::WRONG_ARGS_NUMBER,
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
void util::syntaxError(const Parser& parser) {
    error(ERROR_TYPE::BAD_SYNTAX,
          to_string(parser.getLineNumber()) + ": " + parser.getLine());
}
