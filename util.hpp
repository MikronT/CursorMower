#pragma once
#include <vector>
#include <Windows.h>


enum class ERROR_TYPE {
    WRONG_ARGS_NUMBER = 1,
    FILE_READ_ERROR = 2,
    BAD_SYNTAX = 3,
    ARG_OUT_OF_BOUNDS = 4
};

namespace util {
    int help();
    [[noreturn]] void error(ERROR_TYPE error, const std::string& msg);


    short to_short(int number);
    short to_short(const std::string& text, int fromLine);


    template <typename T> [[nodiscard]] bool contains(const std::vector<T>& vector, T entry) {
        return std::ranges::find(vector, entry) != vector.end();
    }


    short getCoordArgument(const std::vector<std::string>& cells, int line_i);
    void normallizeCoords(const COORD& dims, COORD& point1, COORD& point2);
    void rearrangeCoords(COORD& point1, COORD& point2);
};
