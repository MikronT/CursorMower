#pragma once
#include <string>
#include <vector>
#include "parser.hpp"


enum class ERROR_TYPE {
    WRONG_ARGS_NUMBER = 1,
    FILE_READ_ERROR = 2,
    BAD_SYNTAX = 3,
    ARG_OUT_OF_BOUNDS = 4
};

namespace util {
    int help();

    [[noreturn]] void error(ERROR_TYPE error, const std::string& msg);
    [[noreturn]] void syntaxError(const Parser& parser);


    template <typename T> [[nodiscard]] bool contains(const std::vector<T>& vector, T entry) {
        return std::ranges::find(vector, entry) != vector.end();
    }
};
