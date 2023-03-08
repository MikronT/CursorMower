#include <Windows.h>
#include "ui.hpp"
#include "xString.hpp"

using std::string, std::to_string, std::vector;


auto Point::getCoord() const { return COORD{x, y}; }


vector<Drawable> Rect::draw() {
    const auto line = string(width + margin * 4, ' ');

    vector<Line> lines;
    for (int y = 0; y < height + margin * 2; ++y)
        lines.emplace_back(Line{color, line});

    return vector{Drawable{at, lines}};
}

vector<Drawable> Paragraph::draw() {
    //Add empty line to ensure paragraph has lines
    if (lines.empty())
        writeLine("");

    return vector{Drawable{at, lines}};
}

vector<Drawable> Container::draw() {
    vector<Drawable> out;
    for (const auto& e : elements) {
        auto drawnElement = e->draw();

        for (auto& drawable : drawnElement) {
            if (margin != 0 && e->shouldApplyMargins()) {
                drawable.at.x = static_cast<short>(drawable.at.x + margin * 2);
                drawable.at.y = static_cast<short>(drawable.at.y + margin);
            }

            out.emplace_back(drawable);
        }
    }
    return out;
}


Window& Window::remapColor(const string& key, const string& value) {
    int k = 0; //if (key == "0" || key == "black")
    if (key == "1" || key == "blue") k = 1;
    else if (key == "2" || key == "green") k = 2;
    else if (key == "3" || key == "cyan") k = 3;
    else if (key == "4" || key == "red") k = 4;
    else if (key == "5" || key == "purple") k = 5;
    else if (key == "6" || key == "yellow") k = 6;
    else if (key == "7" || key == "white") k = 7;
    else if (key == "8" || key == "bright-black") k = 8;
    else if (key == "9" || key == "bright-blue") k = 9;
    else if (key == "a" || key == "bright-green") k = 10;
    else if (key == "b" || key == "bright-cyan") k = 11;
    else if (key == "c" || key == "bright-red") k = 12;
    else if (key == "d" || key == "bright-purple") k = 13;
    else if (key == "e" || key == "bright-yellow") k = 14;
    else if (key == "f" || key == "bright-white") k = 15;

    colormap.emplace(k, value);
    colormapUpdated = true;
    return *this;
}
void Window::show(Container& container) {
    //Hide cursor
    cmd.setCursorVisibility(false);

    if (colormapUpdated) {
        colormapUpdated = false;
        cmd.remapColors(colormap);
    }

    const COORD screenDims{
        static_cast<short>(width + container.getMargin() * 4),
        static_cast<short>(height + container.getMargin() * 2)
    };
    if (dimsUpdated) {
        dimsUpdated = false;
        cmd.setScreenDims(screenDims);
    }

    const auto drawnContainer = container.draw();

    for (const auto& [at, lines] : drawnContainer) {
        auto pos = at.getCoord();
        //Fix out of bounds and do a shift to (0;0)
        normallizeCoord(screenDims, pos);

        for (const auto& [color, text] : lines)
            if (text.empty()) {
                //Move to some position manually
                cmd.setColor(color);
                cmd.goTo(pos);
            }
            else {
                //Expand environment variables
                auto out = CommandLine::expandEnvironmentVariables(text);
                //Trim the text to screen size
                out = xString::cut(out, screenDims.X - pos.X);

                //Then actually print text
                cmd.write(out, color, pos);

                //Move the cursor down to print next line
                pos.Y++;
            }
    }

    //Reenable cursor
    cmd.setCursorVisibility(true);
}
