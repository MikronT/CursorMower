#pragma once
#include <iomanip>
#include <random>
#include <sstream>
#include <string>
#include <vector>

using std::ostringstream, std::string, std::vector, std::wstring;


namespace nsStringLib {
    template <typename T> [[nodiscard]] string string_cut(T what, const int to) {
        ostringstream format;
        format << std::setprecision(2) << what;
        auto output = format.str();

        const auto str = output.c_str();
        auto size = 0;

        for (size_t i = 0; i < output.size(); ++i)
            if (size < to) size += (str[i] & 0xc0) != 0x80;
            else return output.substr(0, i);
        return output;
    }

    [[nodiscard]] int string_getSize(const string& of);
    [[nodiscard]] int string_getSize(const char* of);
}