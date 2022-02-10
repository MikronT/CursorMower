#pragma once
#include <Windows.h>


class CommandLine {
    HANDLE console_handle_out;
public:
    static constexpr short COLOR_DEFAULT = 7;


    CommandLine() { console_handle_out = GetStdHandle(STD_OUTPUT_HANDLE); }

    void setColor(short color = 7) const;
    void setConInfo(CONSOLE_SCREEN_BUFFER_INFOEX& info) const;
    void setScreenDims(const COORD& dims) const;

    void goTo(const COORD& pos) const;

    [[nodiscard]] CONSOLE_SCREEN_BUFFER_INFOEX getConInfo() const;
    [[nodiscard]] COORD getScreenDims() const;
};
