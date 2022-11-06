#pragma once
#include <iomanip>
#include <sstream>
#include <vector>

using std::vector, std::wostringstream, std::wstring;


namespace xString {
    [[nodiscard]] vector<wstring> split(const wstring& source, wchar_t delim, int maxTokens = -1);
}
