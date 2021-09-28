#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include "commandLine.hpp"
#include "string.hpp"
#include "utils.hpp"

using namespace std;
using namespace nsString;
using namespace nsUtils;


struct Block {
    COORD coord;
    vector<string> lines;
};


int main(const int arg_count, char** arg_list) {
    if (arg_count == 1 ||
        arg_count == 2 && string(arg_list[1]) == "/help")
        return help();

    if (arg_count != 2)
        error(ERROR_ARGS_COUNT,
              to_string(arg_count - 1));


    const string arg_file = arg_list[1];
    COORD param_dims = {0, 0};
    short param_margin = 0;
    vector<Block> param_actions;


    //Parse input from the file
    ifstream layout;
    layout.open(arg_file);
    if (!layout.is_open())
        error(ERROR_FILE,
              arg_file);

    string line_read;
    bool cursor_moved = true;
    auto cursor1 = COORD{1, 1},
         cursor2 = COORD{1, 1};
    int line_i = 0;

    while (getline(layout, line_read)) {
        line_i++;
        vector<string> cells = string_split(line_read, '=', 2);
        if (cells.empty())
            continue;

        //Check syntax of 1-value parameters
        if (contains(
            vector<string>{
                "screen_width", "screen_height", "screen_margin",
                "cursor1_up", "cursor1_down", "cursor1_left", "cursor1_right",
                "cursor2_up", "cursor2_down", "cursor2_left", "cursor2_right",
                "text"
            },
            cells.at(0))) {
            if (cells.size() != 2)
                error(ERROR_FORMAT, to_string(line_i) + ": " + line_read);
        }


        //Parse parameters
        if (cells.at(0) == "screen_width")
            param_dims.X = to_short(cells.at(1), line_i);

        else if (cells.at(0) == "screen_height")
            param_dims.Y = to_short(cells.at(1), line_i);

        else if (cells.at(0) == "screen_margin")
            param_margin = to_short(cells.at(1), line_i);

        else if (cells.at(0) == "cursor1") {
            vector<string> values = string_split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                error(ERROR_FORMAT, to_string(line_i) + ": " + line_read);

            cursor_moved = true;
            cursor1 = COORD{
                to_short(values.at(0), line_i),
                to_short(values.at(1), line_i)
            };
        } else if (cells.at(0) == "cursor2") {
            vector<string> values = string_split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                error(ERROR_FORMAT, to_string(line_i) + ": " + line_read);

            cursor_moved = true;
            cursor2 = COORD{
                to_short(values.at(0), line_i),
                to_short(values.at(1), line_i)
            };
        } else if (cells.at(0) == "cursor1_up") {
            cursor_moved = true;
            cursor1.Y--;
        } else if (cells.at(0) == "cursor1_down" || cells.at(0) == "skip") {
            cursor_moved = true;
            cursor1.Y++;
        } else if (cells.at(0) == "cursor1_left") {
            cursor_moved = true;
            cursor1.X--;
        } else if (cells.at(0) == "cursor1_right") {
            cursor_moved = true;
            cursor1.X++;
        } else if (cells.at(0) == "cursor2_up") {
            cursor_moved = true;
            cursor2.Y--;
        } else if (cells.at(0) == "cursor2_down") {
            cursor_moved = true;
            cursor2.Y++;
        } else if (cells.at(0) == "cursor2_left") {
            cursor_moved = true;
            cursor2.X--;
        } else if (cells.at(0) == "cursor2_right") {
            cursor_moved = true;
            cursor2.X++;
        } else if (cells.at(0) == "text") {
            if (cursor_moved) {
                cursor_moved = false;
                param_actions.emplace_back(Block{cursor1, {}});
            }

            param_actions
                    .at(param_actions.size() - 1)
                    .lines
                    .emplace_back(cells.at(1));
        } else if (cells.at(0) == "clear") {
            int lines, length;

            if (cells.size() == 1) {
                lines = abs(cursor2.Y - cursor1.Y);
                length = abs(cursor2.X - cursor1.X);
            } else if (cells.at(1) == "screen") {
                lines = param_dims.Y;
                length = param_dims.X;
            } else
                error(ERROR_FORMAT, to_string(line_i) + ": " + line_read);

            auto text = string(length, ' ');

            if (cells.size() == 1)
                param_actions.emplace_back(Block{COORD{1, 1}, {}});

            for (int i = 0; i < lines; i++)
                param_actions
                        .at(param_actions.size() - 1)
                        .lines
                        .emplace_back(text);
        }
    }

    layout.close();


    //Get/set screen dimensions
    auto cmd = CommandLine();

    if (param_dims.X == 0 || param_dims.Y == 0) {
        auto [X, Y] = cmd.getScreenDims();

        param_dims.X = X - param_margin * 2 * 2;
        param_dims.Y = Y - param_margin * 2;
    } else {
        param_dims.X = param_dims.X + param_margin * 2 * 2;
        param_dims.Y = param_dims.Y + param_margin * 2;

        cmd.setScreenDims(param_dims);
    }


    //Write text to screen
    for (auto& [coord, lines] : param_actions) {
        coord = {
            to_short(coord.X + 1 + param_margin * 2),
            to_short(coord.Y + 1 + param_margin)
        };

        if (lines.empty())
            cmd.goTo(coord);
        else
            for (auto& text : lines) {
                if (coord.X < 1 || coord.X > param_dims.X ||
                    coord.Y < 1 || coord.Y > param_dims.Y)
                    error(ERROR_OUT_OF_BOUNDS,
                          to_string(coord.X) + ";" +
                          to_string(coord.Y) +
                          " with bounds of " +
                          to_string(param_dims.X) + ":" +
                          to_string(param_dims.Y));

                cmd.goTo(coord);

                cout << string_cut(text, param_dims.X - coord.X);

                coord = {coord.X, to_short(coord.Y + 1)};
            }
    }
    return 0;
}
