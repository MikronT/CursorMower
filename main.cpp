// ReSharper disable CppClangTidyCppcoreguidelinesNarrowingConversions
// ReSharper disable CppClangTidyBugproneNarrowingConversions
#include <fstream>
#include <map>
#include <sstream>
#include <vector>
#include <Windows.h>
#include "commandLine.hpp"
#include "util.hpp"
#include "xString.hpp"

using std::make_unique;
using std::ifstream;
using std::map, std::string, std::stringstream, std::to_string, std::vector, std::wstring;
using util::getCoordArgument, util::to_short;


struct Line {
    short color;
    string text;
};

struct Block {
    COORD coord;
    vector<Line> lines;
};


const string OPTION_CONSOLE_WIDTH = "console_width";
const string OPTION_CONSOLE_HEIGHT = "console_height";
const string OPTION_CONSOLE_MARGIN = "console_margin";
const string OPTION_CONSOLE_COLOR = "console_color";

const string OPTION_CURSOR1 = "cursor1";
const string OPTION_CURSOR2 = "cursor2";
const string OPTION_CURSOR1_DOWN = "cursor1_down";
const string OPTION_CURSOR2_DOWN = "cursor2_down";
const string OPTION_CURSOR1_DOWN_SHORT = "down";
const string OPTION_CURSOR2_DOWN_SHORT = "down2";
const string OPTION_CURSOR1_LEFT = "cursor1_left";
const string OPTION_CURSOR2_LEFT = "cursor2_left";
const string OPTION_CURSOR1_LEFT_SHORT = "left";
const string OPTION_CURSOR2_LEFT_SHORT = "left2";
const string OPTION_CURSOR1_RIGHT = "cursor1_right";
const string OPTION_CURSOR2_RIGHT = "cursor2_right";
const string OPTION_CURSOR1_RIGHT_SHORT = "right";
const string OPTION_CURSOR2_RIGHT_SHORT = "right2";
const string OPTION_CURSOR1_UP = "cursor1_up";
const string OPTION_CURSOR2_UP = "cursor2_up";
const string OPTION_CURSOR1_UP_SHORT = "up";
const string OPTION_CURSOR2_UP_SHORT = "up2";

const string OPTION_COLOR = "color";

const string OPTION_CLEAR = "clear";
const string OPTION_CLEAR_SCREEN = "screen";
const string OPTION_TEXT = "text";

//Option groups
const vector OPTIONS_CURSOR1_DOWN{OPTION_CURSOR1_DOWN, OPTION_CURSOR1_DOWN_SHORT};
const vector OPTIONS_CURSOR2_DOWN{OPTION_CURSOR2_DOWN, OPTION_CURSOR2_DOWN_SHORT};
const vector OPTIONS_CURSOR1_LEFT{OPTION_CURSOR1_LEFT, OPTION_CURSOR1_LEFT_SHORT};
const vector OPTIONS_CURSOR2_LEFT{OPTION_CURSOR2_LEFT, OPTION_CURSOR2_LEFT_SHORT};
const vector OPTIONS_CURSOR1_RIGHT{OPTION_CURSOR1_RIGHT, OPTION_CURSOR1_RIGHT_SHORT};
const vector OPTIONS_CURSOR2_RIGHT{OPTION_CURSOR2_RIGHT, OPTION_CURSOR2_RIGHT_SHORT};
const vector OPTIONS_CURSOR1_UP{OPTION_CURSOR1_UP, OPTION_CURSOR1_UP_SHORT};
const vector OPTIONS_CURSOR2_UP{OPTION_CURSOR2_UP, OPTION_CURSOR2_UP_SHORT};
//Options requiring an argument
const vector OPTIONS_ARG_REQUIRED{
    OPTION_CONSOLE_WIDTH,
    OPTION_CONSOLE_HEIGHT,
    OPTION_CONSOLE_MARGIN,
    OPTION_CONSOLE_COLOR,
    OPTION_CURSOR1,
    OPTION_CURSOR2
};
//Options with an optional argument
const vector OPTIONS_ARG_OPTIONAL{
    OPTION_CURSOR1_DOWN,
    OPTION_CURSOR2_DOWN,
    OPTION_CURSOR1_DOWN_SHORT,
    OPTION_CURSOR2_DOWN_SHORT,
    OPTION_CURSOR1_LEFT,
    OPTION_CURSOR2_LEFT,
    OPTION_CURSOR1_LEFT_SHORT,
    OPTION_CURSOR2_LEFT_SHORT,
    OPTION_CURSOR1_RIGHT,
    OPTION_CURSOR2_RIGHT,
    OPTION_CURSOR1_RIGHT_SHORT,
    OPTION_CURSOR2_RIGHT_SHORT,
    OPTION_CURSOR1_UP,
    OPTION_CURSOR2_UP,
    OPTION_CURSOR1_UP_SHORT,
    OPTION_CURSOR2_UP_SHORT,
    OPTION_COLOR,
    OPTION_CLEAR,
    OPTION_TEXT
};


const auto COLON = ": ";


int wmain(const int arg_count, wchar_t** arg_list) {
    if (arg_count == 1 ||
        arg_count == 2 && wstring(arg_list[1]) == L"/help")
        return util::help();

    if (arg_count != 2)
        util::error(ERROR_TYPE::WRONG_ARGS_NUMBER, to_string(arg_count - 1));


    auto cmd = make_unique<CommandLine>();

    string arg_file = xString::fromWide(arg_list[1]);
    auto param_dims = cmd->getScreenDims();
    short param_margin = 0;
    map<int, string> param_colors;
    bool param_colors_changed = false;
    vector<Block> param_actions;

    stringstream color_stream;
    short color_last = CommandLine::COLOR_DEFAULT;
    auto cursor1 = COORD{1, 1},
         cursor2 = COORD{1, 1};
    bool cursor_changed = true;


    //Parse input from the file
    ifstream layout;
    layout.open(arg_file);
    if (!layout.is_open())
        util::error(ERROR_TYPE::FILE_READ_ERROR, arg_file);

    string line_read;
    int line_i = 0;

    while (getline(layout, line_read)) {
        line_i++;
        vector<string> cells = xString::split(line_read, '=', 2);
        string cell0 = cells.at(0);

        if (cells.empty())
            continue;

        //Check syntax
        if (util::contains(OPTIONS_ARG_REQUIRED, cell0)) {
            if (cells.size() != 2)
                util::error(ERROR_TYPE::BAD_SYNTAX, to_string(line_i) + COLON + line_read);
        }
        else if (!util::contains(OPTIONS_ARG_OPTIONAL, cell0))
            util::error(ERROR_TYPE::BAD_SYNTAX, to_string(line_i) + COLON + line_read);


        //Parse parameters
        if (cell0 == OPTION_CONSOLE_WIDTH)
            param_dims->X = to_short(cells.at(1), line_i);

        else if (cell0 == OPTION_CONSOLE_HEIGHT)
            param_dims->Y = to_short(cells.at(1), line_i);

        else if (cell0 == OPTION_CONSOLE_MARGIN)
            param_margin = to_short(cells.at(1), line_i);

        else if (cell0 == OPTION_CONSOLE_COLOR) {
            vector<string> values = xString::split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                util::error(ERROR_TYPE::BAD_SYNTAX, to_string(line_i) + COLON + line_read);

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
        else if (cell0 == OPTION_CURSOR1) {
            vector<string> values = xString::split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                util::error(ERROR_TYPE::BAD_SYNTAX, to_string(line_i) + COLON + line_read);

            cursor_changed = true;
            cursor1 = COORD{
                to_short(values.at(0), line_i),
                to_short(values.at(1), line_i)
            };
        }
        else if (cell0 == OPTION_CURSOR2) {
            vector<string> values = xString::split(cells.at(1), ' ', 2);

            if (values.size() < 2)
                util::error(ERROR_TYPE::BAD_SYNTAX, to_string(line_i) + COLON + line_read);

            cursor_changed = true;
            cursor2 = COORD{
                to_short(values.at(0), line_i),
                to_short(values.at(1), line_i)
            };
        }
        else if (util::contains(OPTIONS_CURSOR1_UP, cell0)) {
            cursor_changed = true;
            cursor1.Y -= getCoordArgument(cells, line_i);
        }
        else if (util::contains(OPTIONS_CURSOR2_UP, cell0)) {
            cursor_changed = true;
            cursor2.Y -= getCoordArgument(cells, line_i);
        }
        else if (util::contains(OPTIONS_CURSOR1_DOWN, cell0)) {
            cursor_changed = true;
            cursor1.Y += getCoordArgument(cells, line_i);
        }
        else if (util::contains(OPTIONS_CURSOR2_DOWN, cell0)) {
            cursor_changed = true;
            cursor2.Y += getCoordArgument(cells, line_i);
        }
        else if (util::contains(OPTIONS_CURSOR1_LEFT, cell0)) {
            cursor_changed = true;
            cursor1.X -= getCoordArgument(cells, line_i);
        }
        else if (util::contains(OPTIONS_CURSOR2_LEFT, cell0)) {
            cursor_changed = true;
            cursor2.X -= getCoordArgument(cells, line_i);
        }
        else if (util::contains(OPTIONS_CURSOR1_RIGHT, cell0)) {
            cursor_changed = true;
            cursor1.X += getCoordArgument(cells, line_i);
        }
        else if (util::contains(OPTIONS_CURSOR2_RIGHT, cell0)) {
            cursor_changed = true;
            cursor2.X += getCoordArgument(cells, line_i);
        }
        else if (cell0 == OPTION_COLOR) {
            if (cells.size() > 1) {
                color_stream << cells.at(1);
                color_stream >> std::hex >> color_last;
                color_stream.clear();
            }
            else
                color_last = CommandLine::COLOR_DEFAULT;
        }
        else if (cell0 == OPTION_TEXT) {
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
        else if (cell0 == OPTION_CLEAR) {
            util::rearrangeCoords(cursor1, cursor2);

            int lines = 0, length = 0;

            //Create an empty block
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
            else if (cells.at(1) == OPTION_CLEAR_SCREEN) {
                lines = param_dims->Y + param_margin * 2;
                length = param_dims->X + param_margin * 2 * 2;
                param_actions.emplace_back(Block{
                    COORD{1, 1},
                    {}
                });
            }
            else
                util::error(ERROR_TYPE::BAD_SYNTAX, to_string(line_i) + COLON + line_read);

            //Then add some space
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
            util::normallizeCoords(*param_dims, cursor1, cursor2);
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
                    util::error(ERROR_TYPE::ARG_OUT_OF_BOUNDS,
                                to_string(coord.X) + "; " +
                                to_string(coord.Y) +
                                " with bounds of " +
                                to_string(param_dims->X) + ":" +
                                to_string(param_dims->Y));

                cmd->goTo(coord);
                cmd->setColor(color);

                {
                    //Expand environment variables
                    auto out = CommandLine::expandEnvironmentVariables(text);

                    //Trim the text to screen size
                    out = xString::cut(out, param_dims->X - coord.X);

                    printf("%s", out.data());
                }

                coord = {coord.X, to_short(coord.Y + 1)};
            }
    }
    return 0;
}
