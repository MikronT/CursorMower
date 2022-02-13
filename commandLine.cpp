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
