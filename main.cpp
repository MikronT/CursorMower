// ReSharper disable CppClangTidyCppcoreguidelinesNarrowingConversions
// ReSharper disable CppClangTidyBugproneNarrowingConversions
#include <memory>
#include <vector>
#include "commandLine.hpp"
#include "parser.hpp"
#include "ui.hpp"
#include "util.hpp"
#include "xString.hpp"

using std::make_shared, std::make_unique, std::shared_ptr, std::unique_ptr;
using std::string, std::to_string, std::vector;
using util::contains;


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
const string OPTION_CARET = "caret";

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
const vector OPTIONS_WITH_ARGS{
    OPTION_CONSOLE_WIDTH,
    OPTION_CONSOLE_HEIGHT,
    OPTION_CONSOLE_MARGIN,
    OPTION_CONSOLE_COLOR,
    OPTION_CURSOR1,
    OPTION_CURSOR2
};
//Options with an optional argument
const vector OPTIONS_WITHOUT_ARGS{
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
    OPTION_TEXT,
    OPTION_CARET
};


int wmain(const int arg_count, wchar_t** arg_list) {
    if (arg_count <= 1)
        return util::help();

    if (arg_count > 2)
        util::error(ERROR_TYPE::WRONG_ARGS_NUMBER, to_string(arg_count - 1));

    string arg_file = xString::fromWide(arg_list[1]);
    if (arg_file == "/help")
        return util::help();


    auto lastColor = CommandLine::COLOR_DEFAULT;
    auto lastText = make_shared<Paragraph>();

    const auto window = make_unique<Window>(CommandLine());
    const auto container = make_unique<Container>();
    const auto cursor1 = make_unique<Cursor>();
    const auto cursor2 = make_unique<Cursor>();


    //Parse input from the file
    const auto parser = make_unique<Parser>(arg_file);
    if (!parser->isOpen())
        util::error(ERROR_TYPE::FILE_READ_ERROR, arg_file);

    parser->setSingleKeys(OPTIONS_WITHOUT_ARGS);
    parser->setParamKeys(OPTIONS_WITH_ARGS);

    string lastKey;
    while (parser->readNextLine()) {
        const auto key = parser->getKey();

        //Parse parameters
        if (key == OPTION_CONSOLE_WIDTH) window->setWidth(parser->getShort());
        else if (key == OPTION_CONSOLE_HEIGHT) window->setHeight(parser->getShort());
        else if (key == OPTION_CONSOLE_MARGIN) container->setMargin(parser->getShort());
        else if (key == OPTION_CONSOLE_COLOR) {
            const auto [colorKey, color] = parser->getStrings();
            window->remapColor(colorKey, color);
        }
        else if (key == OPTION_CURSOR1) {
            const auto [x, y] = parser->getShorts();
            cursor1->toX(x).toY(y);
        }
        else if (key == OPTION_CURSOR2) {
            const auto [x, y] = parser->getShorts();
            cursor2->toX(x).toY(y);
        }
        else if (contains(OPTIONS_CURSOR1_DOWN, key)) cursor1->down(parser->getShort(1));
        else if (contains(OPTIONS_CURSOR1_LEFT, key)) cursor1->left(parser->getShort(1));
        else if (contains(OPTIONS_CURSOR1_RIGHT, key)) cursor1->right(parser->getShort(1));
        else if (contains(OPTIONS_CURSOR1_UP, key)) cursor1->up(parser->getShort(1));
        else if (contains(OPTIONS_CURSOR2_DOWN, key)) cursor2->down(parser->getShort(1));
        else if (contains(OPTIONS_CURSOR2_LEFT, key)) cursor2->left(parser->getShort(1));
        else if (contains(OPTIONS_CURSOR2_RIGHT, key)) cursor2->right(parser->getShort(1));
        else if (contains(OPTIONS_CURSOR2_UP, key)) cursor2->up(parser->getShort(1));
        else if (key == OPTION_COLOR) lastColor = parser->getHexColor(CommandLine::COLOR_DEFAULT);
        else if (key == OPTION_CLEAR) {
            shared_ptr<Rect> rect;

            if (parser->getWordsNumber() == 1)
                rect = make_shared<Rect>(*cursor1, *cursor2);

            else if (parser->getString() == OPTION_CLEAR_SCREEN)
                rect = make_shared<Rect>(*window, *container);

            else util::syntaxError(*parser);

            rect->setFillColor(lastColor);
            container->put(rect);
        }
        else if (key == OPTION_TEXT) {
            //Create new paragraph once per block of lines
            if (lastKey != OPTION_TEXT) {
                lastText = make_shared<Paragraph>(*cursor1);
                container->put(lastText);
            }

            //Empty line returned by getString is allowed intentionally
            lastText->setPenColor(lastColor)
                    .writeLine(parser->getString());

            //Move cursor down with the next paragraph for later use
            cursor1->down();
        }
        else if (key == OPTION_CARET) {
            const auto caret = make_shared<Paragraph>(*cursor1);
            caret->setPenColor(lastColor);
            container->put(caret);
        }

        //Save last option key
        lastKey = key;
    }

    window->show(*container);
    return 0;
}
