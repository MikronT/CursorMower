#include <ios>
#include <sstream>
#include <string>
#include "commandLine.hpp"
#include "xString.hpp"

using std::make_unique, std::unique_ptr;
using std::hex, std::map, std::ostringstream, std::string, std::stringstream, std::to_string, std::wstring;


void CommandLine::setColor(const short color) const {
    SetConsoleTextAttribute(console_handle_out, color);
}
void CommandLine::setConInfo(CONSOLE_SCREEN_BUFFER_INFOEX& info) const {
    info.srWindow.Bottom++;
    SetConsoleScreenBufferInfoEx(console_handle_out, &info);
}
void CommandLine::setScreenDims(const COORD& dims) const {
    const auto info = getConInfo();

    info->dwSize = dims;
    info->dwMaximumWindowSize = dims;
    info->srWindow = {0, 0, dims.X, dims.Y};

    setConInfo(*info);
}
void CommandLine::remapColors(const map<int, string>& colorMap) const {
    const auto dims = getScreenDims();
    const auto info = getConInfo();

    stringstream stream;

    for (auto& [key, color] : colorMap) {
        short red, green, blue;

        stream << hex << color.substr(0, 2);
        stream >> red;
        stream.clear();
        stream << hex << color.substr(2, 2);
        stream >> green;
        stream.clear();
        stream << hex << color.substr(4, 2);
        stream >> blue;
        stream.clear();

        info->ColorTable[key] = RGB(red, green, blue);
    }

    setConInfo(*info);
}

void CommandLine::goTo(const COORD& pos) const {
    SetConsoleCursorPosition(console_handle_out, pos);
}

unique_ptr<CONSOLE_SCREEN_BUFFER_INFOEX> CommandLine::getConInfo() const {
    auto info = make_unique<CONSOLE_SCREEN_BUFFER_INFOEX>();
    info->cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);

    GetConsoleScreenBufferInfoEx(console_handle_out, info.get());
    return info;
}
unique_ptr<COORD> CommandLine::getScreenDims() const {
    auto [left, top, right, bottom] = getConInfo()->srWindow;

    auto output = make_unique<COORD>(
        static_cast<short>(right - left + 1),
        static_cast<short>(bottom - top + 1)
    );
    return output;
}


string CommandLine::getEnvVar(const string& name) {
    const auto buffer = make_unique<wchar_t[]>(LINE_SIZE);
    GetEnvironmentVariableW(
        xString::toWide(name).data(),
        buffer.get(),
        LINE_SIZE);

    string out = xString::fromWide(buffer.get());
    return out;
}
string CommandLine::expandEnvironmentVariables(const string& in) {
    ostringstream out;

    size_t offset = 0, i, percent1 = 44170;

    auto expanded = false;
    while ((i = in.find('%', offset)) != string::npos) {
        expanded = true;
        //Look for the 1st appearance of %
        if (percent1 == 44170) {
            percent1 = i; //First appearance of %

            //Save everything before the char to the stream
            out << in.substr(offset, i - offset);

            offset = i + 1;
        }
        else {
            const size_t percent2 = i; //Second appearance of %
            offset = i + 1;

            auto variable = in.substr(percent1 + 1, percent2 - percent1 - 1);
            out << getEnvVar(variable);

            percent1 = 44170;
        }
    }

    out << in.substr(offset, in.size());
    if (expanded)
        return expandEnvironmentVariables(out.str());
    return out.str();
}
