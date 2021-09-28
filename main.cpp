#include <fstream>
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
        auto line_coord = COORD{0, 0};

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

                line_coord = COORD{
                    to_short(cell.at(1), line_i),
                    to_short(cell.at(2), line_i)
                };
            }

            if (cell.at(0) == "dims")
                arg_dims = line_coord;

            else if (cell.at(0) == "margins")
                arg_margins = line_coord;

            else if (cell.at(0) == "goto")
                arg_blocks.emplace_back(Block{line_coord, {}});

            else if (cell.at(0) == "clear") {
                auto lines =
                    cell.size() == 1 ?
                        arg_dims.Y :
                        cell.size() == 2 ?
                            1 :
                            stoi(cell.at(2)) - arg_blocks.at(arg_blocks.size() - 1).coord.Y;
                auto length =
                    cell.size() == 1 ?
                        arg_dims.X :
                        to_short(cell.at(1), line_i);
                auto text = string(length, ' ');

                if (cell.size() == 1)
                    arg_blocks.emplace_back(Block{COORD{0, 0}, {}});

                for (int i = 0; i < lines; i++)
                    arg_blocks
                        .at(arg_blocks.size() - 1)
                        .lines
                        .emplace_back(text);
            } else if (cell.at(0) == "text") {
                if (cell.size() < 2)
                    error(ERROR_FORMAT, to_string(line_i) + ": " + line);

                arg_blocks
                    .at(arg_blocks.size() - 1)
                    .lines
                    .emplace_back(cell.at(1));
            }
        }
    }

    layout.close();


    //Get/set screen dimensions
    auto cmd = CommandLine();

    if (arg_dims.X == 0 || arg_dims.Y == 0) {
        auto [X, Y] = cmd.getScreenDims();

        arg_dims.X = X - arg_margins.X * 2;
        arg_dims.Y = Y - arg_margins.Y * 2;
    } else {
        arg_dims.X = arg_dims.X + arg_margins.X * 2;
        arg_dims.Y = arg_dims.Y + arg_margins.Y * 2;

        cmd.setScreenDims(arg_dims);
    }


    //Write text to screen
    for (auto& [coord, lines] : arg_blocks) {
        coord = {
            to_short(coord.X + arg_margins.X),
            to_short(coord.Y + arg_margins.Y)
        };

        if (lines.empty())
            cmd.goTo(coord);
        else
            for (auto& i : lines) {
                if (coord.X < 0 || coord.X >= arg_dims.X ||
                    coord.Y < 0 || coord.Y >= arg_dims.Y)
                    error(ERROR_OUT_OF_BOUNDS,
                          to_string(coord.X) + ";" +
                          to_string(coord.Y) +
                          " with bounds of " +
                          to_string(arg_dims.X) + ":" +
                          to_string(arg_dims.Y));

                cmd.goTo(coord);

                cout << string_cut(i, arg_dims.X - coord.X);

                coord = {coord.X, to_short(coord.Y + 1)};
            }
    }
    return 0;
}
