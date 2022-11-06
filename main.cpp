// ReSharper disable CppClangTidyCppcoreguidelinesNarrowingConversions
// ReSharper disable CppClangTidyBugproneNarrowingConversions
#include <fstream>
#include <iostream>
#include "commandLine.hpp"
#include "string.hpp"
#include "utils.hpp"

using std::to_wstring, std::wcout;
using nsString::string_cut, nsString::string_split;
using namespace nsUtils;


struct Line {
    short color;
    wstring text;
};

struct Block {
    COORD coord;
    vector<Line> lines;
};


const auto COLON = L": ";


int wmain(const int arg_count, wchar_t** arg_list) {
    if (arg_count == 1 ||
        arg_count == 2 && wstring(arg_list[1]) == L"/help")
        return help();

    if (arg_count != 2)
        error(ERROR_ARGS_COUNT, to_wstring(arg_count - 1));


    auto cmd = make_unique<CommandLine>();

    const wstring arg_file = arg_list[1];
    auto param_dims = cmd->getScreenDims();
    short param_margin = 0;
    map<int, wstring> param_colors;
    bool param_colors_changed = false;
    vector<Block> param_actions;

    std::wstringstream color_stream;
    short color_last = CommandLine::COLOR_DEFAULT;
    auto cursor1 = COORD{1, 1},
         cursor2 = COORD{1, 1};
    bool cursor_changed = true;


    //Parse input from the file
    std::wifstream layout;
    layout.open(arg_file);
    if (!layout.is_open())
        error(ERROR_FILE, arg_file);

    wstring line_read;
    int line_i = 0;

    while (std::getline(layout, line_read)) {
        line_i++;
        vector<wstring> cells = string_split(line_read, '=', 2);
        wstring cell0 = cells.at(0);

        if (cells.empty())
            continue;

        //Check syntax
        if (contains(
            vector<wstring>{
                //Options with required argument
                L"console_width", L"console_height", L"console_margin", L"console_color",
                L"cursor1", L"cursor2"
            }, cell0)) {
            if (cells.size() != 2)
                error(ERROR_SYNTAX, to_wstring(line_i) + COLON + line_read);
        }
        else if (!contains(
            vector<wstring>{
                //Options with optional arguments
                L"cursor1_up", L"cursor1_down", L"cursor1_left", L"cursor1_right", L"up", L"down", L"left", L"right",
                L"cursor2_up", L"cursor2_down", L"cursor2_left", L"cursor2_right", L"up2", L"down2", L"left2", L"right2",
                L"color",
                L"text", L"clear"
            }, cell0))
            error(ERROR_SYNTAX, to_wstring(line_i) + COLON + line_read);


        //Parse parameters
        if (cell0 == L"console_width")
            param_dims->X = to_short(cells.at(1), line_i);

        else if (cell0 == L"console_height")
            param_dims->Y = to_short(cells.at(1), line_i);

        else if (cell0 == L"console_margin")
            param_margin = to_short(cells.at(1), line_i);

        else if (cell0 == L"console_color") {
            vector<wstring> values = string_split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                error(ERROR_SYNTAX, to_wstring(line_i) + COLON + line_read);

            // ReSharper disable once CppTooWideScopeInitStatement
            wstring key = values.at(0), v = values.at(1);
            param_colors_changed = true;
                 if (key == L"0" || key == L"black")         param_colors.emplace(0, v);
            else if (key == L"1" || key == L"blue")          param_colors.emplace(1, v);
            else if (key == L"2" || key == L"green")         param_colors.emplace(2, v);
            else if (key == L"3" || key == L"cyan")          param_colors.emplace(3, v);
            else if (key == L"4" || key == L"red")           param_colors.emplace(4, v);
            else if (key == L"5" || key == L"purple")        param_colors.emplace(5, v);
            else if (key == L"6" || key == L"yellow")        param_colors.emplace(6, v);
            else if (key == L"7" || key == L"white")         param_colors.emplace(7, v);
            else if (key == L"8" || key == L"bright-black")  param_colors.emplace(8, v);
            else if (key == L"9" || key == L"bright-blue")   param_colors.emplace(9, v);
            else if (key == L"a" || key == L"bright-green")  param_colors.emplace(10, v);
            else if (key == L"b" || key == L"bright-cyan")   param_colors.emplace(11, v);
            else if (key == L"c" || key == L"bright-red")    param_colors.emplace(12, v);
            else if (key == L"d" || key == L"bright-purple") param_colors.emplace(13, v);
            else if (key == L"e" || key == L"bright-yellow") param_colors.emplace(14, v);
            else if (key == L"f" || key == L"bright-white")  param_colors.emplace(15, v);
        }
        else if (cell0 == L"cursor1") {
            vector<wstring> values = string_split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                error(ERROR_SYNTAX, to_wstring(line_i) + COLON + line_read);

            cursor_changed = true;
            cursor1 = COORD{
                to_short(values.at(0), line_i),
                to_short(values.at(1), line_i)
            };
        }
        else if (cell0 == L"cursor2") {
            vector<wstring> values = string_split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                error(ERROR_SYNTAX, to_wstring(line_i) + COLON + line_read);

            cursor_changed = true;
            cursor2 = COORD{
                to_short(values.at(0), line_i),
                to_short(values.at(1), line_i)
            };
        }
        else if (cell0 == L"cursor1_up" || cell0 == L"up") {
            cursor_changed = true;
            cursor1.Y -= cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == L"cursor2_up" || cell0 == L"up2") {
            cursor_changed = true;
            cursor2.Y -= cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == L"cursor1_down" || cell0 == L"down") {
            cursor_changed = true;
            cursor1.Y += cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == L"cursor2_down" || cell0 == L"down2") {
            cursor_changed = true;
            cursor2.Y += cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == L"cursor1_left" || cell0 == L"left") {
            cursor_changed = true;
            cursor1.X -= cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == L"cursor2_left" || cell0 == L"left2") {
            cursor_changed = true;
            cursor2.X -= cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == L"cursor1_right" || cell0 == L"right") {
            cursor_changed = true;
            cursor1.X += cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == L"cursor2_right" || cell0 == L"right2") {
            cursor_changed = true;
            cursor2.X += cells.size() == 1 ?
                             1 :
                             to_short(cells.at(1), line_i);
        }
        else if (cell0 == L"color") {
            if (cells.size() > 1) {
                color_stream << cells.at(1);
                color_stream >> std::hex >> color_last;
                color_stream.clear();
            }
            else
                color_last = CommandLine::COLOR_DEFAULT;
        }
        else if (cell0 == L"text") {
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
                            L"" : //Print no text (just move cursor)
                            cells.at(1)
                    });
        }
        else if (cell0 == L"clear") {
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
            else if (cells.at(1) == L"screen") {
                lines = param_dims->Y + param_margin * 2;
                length = param_dims->X + param_margin * 2 * 2;
                param_actions.emplace_back(Block{
                    COORD{1, 1},
                    {}
                });
            }
            else
                error(ERROR_SYNTAX, to_wstring(line_i) + COLON + line_read);

            auto text = wstring(length, ' ');

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
                          to_wstring(coord.X) + L";" +
                          to_wstring(coord.Y) +
                          L" with bounds of " +
                          to_wstring(param_dims->X) + L":" +
                          to_wstring(param_dims->Y));

                cmd->goTo(coord);
                cmd->setColor(color);
                wcout << string_cut(text, param_dims->X - coord.X);

                coord = {coord.X, to_short(coord.Y + 1)};
            }
    }
    return 0;
}
