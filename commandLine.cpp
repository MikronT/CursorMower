#include "commandLine.hpp"

using namespace std;


void CommandLine::setConInfo(CONSOLE_SCREEN_BUFFER_INFOEX& info) const {
    SetConsoleScreenBufferInfoEx(console_handle_out, &info);
}
void CommandLine::setScreenDims(COORD& dims) const {
    CONSOLE_SCREEN_BUFFER_INFOEX info = getConInfo();

    info.dwSize = dims;
    info.srWindow = {0, 0, dims.X, dims.Y};
    setConInfo(info);

    //Do it again to make command line do what it should
    info = getConInfo();
    info.srWindow = {0, 0, dims.X, dims.Y};
    setConInfo(info);
}

void CommandLine::goTo(COORD& pos) const { SetConsoleCursorPosition(console_handle_out, pos); }

CONSOLE_SCREEN_BUFFER_INFOEX CommandLine::getConInfo() const {
    CONSOLE_SCREEN_BUFFER_INFOEX console_info;
    console_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx(console_handle_out, &console_info);
    return console_info;
}
COORD CommandLine::getScreenDims() const {
    CONSOLE_SCREEN_BUFFER_INFOEX info = getConInfo();
    auto& [left, top, right, bottom] = info.srWindow;

    return COORD{
        static_cast<short>(right - left + 1),
        static_cast<short>(bottom - top + 1)
    };
}
