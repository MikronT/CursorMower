#include <thread>
#include "commandLine.hpp"


void CommandLine::setColor(const short color) const {
    SetConsoleTextAttribute(console_handle_out, color);
}
void CommandLine::setConInfo(CONSOLE_SCREEN_BUFFER_INFOEX& info) const {
    SetConsoleScreenBufferInfoEx(console_handle_out, &info);
}
void CommandLine::setScreenDims(const COORD& dims) const {
    auto info = getConInfo();

    info->dwSize = dims;
    info->srWindow = {0, 0, dims.X, dims.Y};
    setConInfo(*info);

    //Do it again to make command line do what it should
    info = getConInfo();
    info->srWindow = {0, 0, dims.X, dims.Y};
    setConInfo(*info);
}
void CommandLine::remapColors(const map<char, string>& colorMap) const {
    const auto dims = getScreenDims();
    const auto info = getConInfo();

    std::stringstream stream;

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

        if ('0' <= key && key <= '9')
            info->ColorTable[key - 48] = RGB(red, green, blue);
        else if ('A' <= key && key <= 'F')
            info->ColorTable[key - 55] = RGB(red, green, blue);
        else if ('a' <= key && key <= 'f')
            info->ColorTable[key - 87] = RGB(red, green, blue);
    }
    setConInfo(*info);

    info->srWindow = {0, 0, dims->X, dims->Y};
    std::this_thread::sleep_for(std::chrono::milliseconds(3));
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
