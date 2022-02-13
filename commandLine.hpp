#pragma once
#include <memory>
#include <Windows.h>

using std::make_unique, std::unique_ptr;


class CommandLine {
    HANDLE console_handle_out;
public:
    static constexpr short COLOR_DEFAULT = 7;


    CommandLine() :
        console_handle_out(GetStdHandle(STD_OUTPUT_HANDLE)) {}
    CommandLine(const CommandLine& that) = delete;
    CommandLine(CommandLine&& that) noexcept = delete;
    CommandLine& operator=(const CommandLine& that) = delete;
    CommandLine& operator=(CommandLine&& that) noexcept = delete;
    ~CommandLine() = default;


    void setColor(short color = 7) const;
    void setConInfo(CONSOLE_SCREEN_BUFFER_INFOEX& info) const;
    void setScreenDims(const COORD& dims) const;

    void goTo(const COORD& pos) const;

    [[nodiscard]] unique_ptr<CONSOLE_SCREEN_BUFFER_INFOEX> getConInfo() const;
    [[nodiscard]] unique_ptr<COORD> getScreenDims() const;
};
