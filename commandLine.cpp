#include "commandLine.hpp"

using namespace std;
using namespace nsCommandLineLib;


void nsCommandLineLib::setConInfo(HANDLE& console_out, CONSOLE_SCREEN_BUFFER_INFOEX& info) {
    SetConsoleScreenBufferInfoEx(console_out, &info);
}

CONSOLE_SCREEN_BUFFER_INFOEX nsCommandLineLib::getConInfo(HANDLE& console_out) {
    CONSOLE_SCREEN_BUFFER_INFOEX console_info;
    console_info.cbSize = sizeof(CONSOLE_SCREEN_BUFFER_INFOEX);
    GetConsoleScreenBufferInfoEx(console_out, &console_info);
    return console_info;
}