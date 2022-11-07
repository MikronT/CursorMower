#include <sstream>
#include "commandLine.hpp"


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
void CommandLine::remapColors(const map<int, wstring>& colorMap) const {
    const auto dims = getScreenDims();
    const auto info = getConInfo();

    std::wstringstream stream;

    for (auto& [key, color] : colorMap) {
        short red, green, blue;

        stream << std::hex << color.substr(0, 2);
        stream >> red;
        stream.clear();
        stream << std::hex << color.substr(2, 2);
        stream >> green;
        stream.clear();
        stream << std::hex << color.substr(4, 2);
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


wstring CommandLine::getEnvVar(const wstring& name) {
    const auto buffer = make_unique<wchar_t[]>(LINE_SIZE);
    GetEnvironmentVariableW(name.data(), buffer.get(), LINE_SIZE);
    return buffer.get();
}
wstring CommandLine::expandEnvironmentVariables(const wstring& in) {
    std::wostringstream out;

    size_t offset = 0, i, percent1 = 44170;

    while ((i = in.find('%', offset)) != wstring::npos) {
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
    return out.str();
}
