#pragma once
#include <iomanip>
#include <sstream>
#include <vector>


namespace xString {
    std::string fromWide(const std::wstring& text);
    std::wstring toWide(const std::string& text);


    template <typename T> [[nodiscard]] std::string cut(T what, const size_t to) {
        std::ostringstream format;
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

    [[nodiscard]] std::vector<std::string> split(const std::string& source, char delim, int maxTokens = -1);
}
