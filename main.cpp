#include <algorithm>
#include <iostream>
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


    return 0;
}
