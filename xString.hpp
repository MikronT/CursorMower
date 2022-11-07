#pragma once
#include <iomanip>
#include <sstream>
#include <vector>

using std::vector, std::wostringstream, std::wstring;


namespace xString {
    template <typename T> [[nodiscard]] wstring cut(T what, const size_t to) {
        wostringstream format;
        format << std::setprecision(2) << what;
        const auto output = format.str();

        const auto str = output.c_str();
        size_t size = 0;

        for (size_t i = 0; i < output.size(); i++)
            if (size < to)
                size += (str[i] & 0xc0) != 0x80;
            else
                return output.substr(0, i);
        return output;
    }

    [[nodiscard]] vector<wstring> split(const wstring& source, wchar_t delim, int maxTokens = -1);
}
