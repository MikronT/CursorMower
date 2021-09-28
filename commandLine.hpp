#pragma once
#include <Windows.h>


class CommandLine {
    HANDLE console_handle_out;
public:
    CommandLine() { console_handle_out = GetStdHandle(STD_OUTPUT_HANDLE); }

    void setConInfo(CONSOLE_SCREEN_BUFFER_INFOEX& info) const;
    void setScreenDims(COORD& dims) const;

    void goTo(COORD& pos) const;

    [[nodiscard]] CONSOLE_SCREEN_BUFFER_INFOEX getConInfo() const;
    [[nodiscard]] COORD getScreenDims() const;
};
