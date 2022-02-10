#pragma once
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::ostringstream, std::string, std::vector, std::wstring;


namespace nsString {
    template <typename T> [[nodiscard]] string string_cut(T what, const int to) {
        ostringstream format;
        format << std::setprecision(2) << what;
        auto output = format.str();

        const auto str = output.c_str();
        auto size = 0;

        for (size_t i = 0; i < output.size(); i++)
            if (size < to)
                size += (str[i] & 0xc0) != 0x80;
            else
                return output.substr(0, i);
        return output;
    }

    [[nodiscard]] int string_getSize(const string& of);
    [[nodiscard]] int string_getSize(const char* of);

    [[nodiscard]] vector<string> string_split(vector<char>& source, char delim, int maxTokens = -1);
    [[nodiscard]] vector<string> string_split(const string& source, char delim, int maxTokens = -1);
}
