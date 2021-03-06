// ReSharper disable CppClangTidyCppcoreguidelinesNarrowingConversions
// ReSharper disable CppClangTidyBugproneNarrowingConversions
#include <fstream>
#include <iostream>
#include "commandLine.hpp"
#include "string.hpp"
#include "utils.hpp"

using std::to_string;
using nsString::string_cut, nsString::string_split;
using namespace nsUtils;


struct Line {
    short color;
    string text;
};

struct Block {
    COORD coord;
    vector<Line> lines;
};


int main(const int arg_count, char** arg_list) {
    if (arg_count == 1 ||
        arg_count == 2 && string(arg_list[1]) == "/help")
        return help();

    if (arg_count != 2)
        error(ERROR_ARGS_COUNT, to_string(arg_count - 1));


    auto cmd = make_unique<CommandLine>();

    const string arg_file = arg_list[1];
    auto param_dims = cmd->getScreenDims();
    short param_margin = 0;
    map<int, string> param_colors;
    bool param_colors_changed = false;
    vector<Block> param_actions;

    std::stringstream color_stream;
    short color_last = CommandLine::COLOR_DEFAULT;
    auto cursor1 = COORD{1, 1},
         cursor2 = COORD{1, 1};
    bool cursor_changed = true;


    //Parse input from the file
    std::ifstream layout;
    layout.open(arg_file);
    if (!layout.is_open())
        error(ERROR_FILE, arg_file);

    string line_read;
    int line_i = 0;

    while (getline(layout, line_read)) {
        line_i++;
        vector<string> cells = string_split(line_read, '=', 2);
        string cell0 = cells.at(0);

        if (cells.empty())
            continue;

        //Check syntax
        if (contains(
            vector<string>{
                //Options with required argument
                "console_width", "console_height", "console_margin", "console_color",
                "cursor1", "cursor2"
            }, cell0)) {
            if (cells.size() != 2)
                error(ERROR_SYNTAX, to_string(line_i) + ": " + line_read);
        }
        else if (!contains(
            vector<string>{
                //Options with optional arguments
                "cursor1_up", "cursor1_down", "cursor1_left", "cursor1_right", "up", "down", "left", "right",
                "cursor2_up", "cursor2_down", "cursor2_left", "cursor2_right", "up2", "down2", "left2", "right2",
                "color",
                "text", "clear"
            }, cell0))
            error(ERROR_SYNTAX, to_string(line_i) + ": " + line_read);


        //Parse parameters
        if (cell0 == "console_width")
            param_dims->X = to_short(cells.at(1), line_i);

        else if (cell0 == "console_height")
            param_dims->Y = to_short(cells.at(1), line_i);

        else if (cell0 == "console_margin")
            param_margin = to_short(cells.at(1), line_i);

        else if (cell0 == "console_color") {
            vector<string> values = string_split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                error(ERROR_SYNTAX, to_string(line_i) + ": " + line_read);

            // ReSharper disable once CppTooWideScopeInitStatement
            string key = values.at(0), v = values.at(1);
            param_colors_changed = true;
                 if (key == "0" || key == "black")         param_colors.emplace(0, v);
            else if (key == "1" || key == "blue")          param_colors.emplace(1, v);
            else if (key == "2" || key == "green")         param_colors.emplace(2, v);
            else if (key == "3" || key == "cyan")          param_colors.emplace(3, v);
            else if (key == "4" || key == "red")           param_colors.emplace(4, v);
            else if (key == "5" || key == "purple")        param_colors.emplace(5, v);
            else if (key == "6" || key == "yellow")        param_colors.emplace(6, v);
            else if (key == "7" || key == "white")         param_colors.emplace(7, v);
            else if (key == "8" || key == "bright-black")  param_colors.emplace(8, v);
            else if (key == "9" || key == "bright-blue")   param_colors.emplace(9, v);
            else if (key == "a" || key == "bright-green")  param_colors.emplace(10, v);
            else if (key == "b" || key == "bright-cyan")   param_colors.emplace(11, v);
            else if (key == "c" || key == "bright-red")    param_colors.emplace(12, v);
            else if (key == "d" || key == "bright-purple") param_colors.emplace(13, v);
            else if (key == "e" || key == "bright-yellow") param_colors.emplace(14, v);
            else if (key == "f" || key == "bright-white")  param_colors.emplace(15, v);
        }
        else if (cell0 == "cursor1") {
            vector<string> values = string_split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                error(ERROR_SYNTAX, to_string(line_i) + ": " + line_read);

            cursor_changed = true;
            cursor1 = COORD{
                to_short(values.at(0), line_i),
                to_short(values.at(1), line_i)
            };
        }
        else if (cell0 == "cursor2") {
            vector<string> values = string_split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                error(ERROR_SYNTAX, to_string(line_i) + ": " + line_read);

            cursor_changed = true;
            cursor2 = COORD{
                to_short(values.at(0), line_i),
                to_short(values.at(1), line_i)
            };
        }
        else if (cell0 == "cursor1_up" || cell0 == "up") {
            cursor_changed = true;
            cursor1.Y -= cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == "cursor2_up" || cell0 == "up2") {
            cursor_changed = true;
            cursor2.Y -= cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == "cursor1_down" || cell0 == "down") {
            cursor_changed = true;
            cursor1.Y += cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == "cursor2_down" || cell0 == "down2") {
            cursor_changed = true;
            cursor2.Y += cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == "cursor1_left" || cell0 == "left") {
            cursor_changed = true;
            cursor1.X -= cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == "cursor2_left" || cell0 == "left2") {
            cursor_changed = true;
            cursor2.X -= cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == "cursor1_right" || cell0 == "right") {
            cursor_changed = true;
            cursor1.X += cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == "cursor2_right" || cell0 == "right2") {
            cursor_changed = true;
            cursor2.X += cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == "color") {
            if (cells.size() > 1) {
                color_stream << cells.at(1);
                color_stream >> std::hex >> color_last;
                color_stream.clear();
            }
            else
                color_last = CommandLine::COLOR_DEFAULT;
        }
        else if (cell0 == "text") {
            if (cursor_changed) {
                cursor_changed = false;
                param_actions.emplace_back(Block{
                    COORD{
                        //Add margins
                        to_short(cursor1.X + param_margin * 2),
                        to_short(cursor1.Y + param_margin)
                    },
                    {}
                });
            }
            cursor1.Y++;

            param_actions
                    .at(param_actions.size() - 1)
                    .lines
                    .emplace_back(Line{
                        color_last,
                        cells.size() == 1 ?
                            "" : //Print no text (just move cursor)
                            cells.at(1)
                    });
        }
        else if (cell0 == "clear") {
            rearrangeCoords(cursor1, cursor2);

            int lines = 0, length = 0;

            if (cells.size() == 1) {
                lines = abs(cursor2.Y - cursor1.Y);
                length = abs(cursor2.X - cursor1.X);
                param_actions.emplace_back(Block{
                    COORD{
                        //Add margins there
                        to_short(cursor1.X + param_margin * 2),
                        to_short(cursor1.Y + param_margin)
                    },
                    {}
                });
            }
            else if (cells.at(1) == "screen") {
                lines = param_dims->Y + param_margin * 2;
                length = param_dims->X + param_margin * 2 * 2;
                param_actions.emplace_back(Block{
                    COORD{1, 1},
                    {}
                });
            }
            else
                error(ERROR_SYNTAX, to_string(line_i) + ": " + line_read);

            auto text = string(length, ' ');

            for (int i = 0; i < lines; i++)
                param_actions
                        .at(param_actions.size() - 1)
                        .lines
                        .emplace_back(Line{
                            color_last,
                            text
                        });
        }

        if (cursor_changed)
            normallizeCoords(*param_dims, cursor1, cursor2);
    }

    layout.close();


    //Get/set screen dimensions
    if (param_dims->X == 0 || param_dims->Y == 0) {
        auto windowDims = cmd->getScreenDims();

        //Double margin for X-axis
        param_dims->X = windowDims->X - param_margin * 2 * 2;
        param_dims->Y = windowDims->Y - param_margin * 2;
    }
    else {
        param_dims->X += param_margin * 2 * 2;
        param_dims->Y += param_margin * 2;

        cmd->setScreenDims(*param_dims);
    }


    //Apply color remapping
    if (param_colors_changed)
        cmd->remapColors(param_colors);


    //Write text to screen
    for (auto& [coord, lines] : param_actions) {
        coord = {
            //-1 means user coord style correction
            to_short(coord.X - 1),
            to_short(coord.Y - 1)
        };

        if (lines.empty())
            cmd->goTo(coord);
        else
            for (auto& [color, text] : lines) {
                if (coord.X < 0 || coord.X >= param_dims->X ||
                    coord.Y < 0 || coord.Y >= param_dims->Y)
                    error(ERROR_OUT_OF_BOUNDS,
                          to_string(coord.X) + ";" +
                          to_string(coord.Y) +
                          " with bounds of " +
                          to_string(param_dims->X) + ":" +
                          to_string(param_dims->Y));

                cmd->goTo(coord);
                cmd->setColor(color);
                std::cout << string_cut(text, param_dims->X - coord.X);

                coord = {coord.X, to_short(coord.Y + 1)};
            }
    }
    return 0;
}
