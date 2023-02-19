#pragma once
#include <vector>
#include <Windows.h>

using std::vector, std::wstring;


namespace nsUtils {
    constexpr int
            ERROR_ARGS_COUNT = 1,
            ERROR_FILE = 2,
            ERROR_SYNTAX = 3,
            ERROR_OUT_OF_BOUNDS = 4;

    int help();
    [[noreturn]] void error(int error, const wstring& msg);


    short to_short(int number);
    short to_short(const wstring& text, int fromLine);


    template <typename T> [[nodiscard]] bool contains(const vector<T>& vector, T entry) {
        return std::ranges::find(vector, entry) != vector.end();
    }


    short getCoordArgument(const vector<wstring>& cells, int line_i);
    void normallizeCoords(const COORD& dims, COORD& point1, COORD& point2);
    void rearrangeCoords(COORD& point1, COORD& point2);
};
