#pragma once
#include <map>
#include <memory>
#include <string>
#include <Windows.h>


class CommandLine {
    HANDLE console_handle_out;
public:
    static constexpr int LINE_SIZE = 8192;
    static constexpr short COLOR_DEFAULT = 7;


    CommandLine() :
        console_handle_out(GetStdHandle(STD_OUTPUT_HANDLE)) {}
    CommandLine(const CommandLine& that) = default;
    CommandLine(CommandLine&& that) noexcept = default;
    CommandLine& operator=(const CommandLine& that) = default;
    CommandLine& operator=(CommandLine&& that) noexcept = default;
    ~CommandLine() = default;


    void setColor(short color = 7) const;
    void setCursorVisibility(bool value) const;
    void setConInfo(CONSOLE_SCREEN_BUFFER_INFOEX& info) const;
    void setScreenDims(const COORD& dims) const;
    void remapColors(const std::map<int, std::string>& colorMap) const;

    void goTo(const COORD& pos) const;
    void write(const std::string& text, short color, const COORD& pos) const;

    [[nodiscard]] std::unique_ptr<CONSOLE_SCREEN_BUFFER_INFOEX> getConInfo() const;
    [[nodiscard]] std::unique_ptr<COORD> getScreenDims() const;


    [[nodiscard]] static std::string getEnvVar(const std::string& name);
    [[nodiscard]] static std::string expandEnvironmentVariables(const std::string& in);
};
