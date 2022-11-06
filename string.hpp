#pragma once
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

using std::vector, std::wstring;


namespace nsString {
    template <typename T> [[nodiscard]] wstring string_cut(T what, const int to) {
        std::wostringstream format;
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

    [[nodiscard]] int string_getSize(const wstring& of);
    [[nodiscard]] int string_getSize(const wchar_t* of);

    [[nodiscard]] vector<wstring> string_split(vector<char>& source, wchar_t delim, int maxTokens = -1);
    [[nodiscard]] vector<wstring> string_split(const wstring& source, wchar_t delim, int maxTokens = -1);
}
