#pragma once
#include <string>
#include <vector>

using std::vector, std::wstring;


namespace xString {
    [[nodiscard]] vector<wstring> split(const wstring& source, wchar_t delim, int maxTokens = -1);
}
