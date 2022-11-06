#include <string>
#include "xString.hpp"


#pragma optimize("", off)
vector<wstring> xString::split(const wstring& source, const wchar_t delim, int maxTokens) {
    vector<wstring> output;
    auto input(source);

    wchar_t* token_next = {};
    auto* token = wcstok_s(input.data(), &delim, &token_next);

    do {
        maxTokens--;

        if (token != nullptr) {
            if (maxTokens != 0)
                output.emplace_back(token);

            else if (token_next != nullptr) {
                const auto rest = wstring(token_next);
                output.emplace_back(wstring(token) + (rest.empty() ? L"" : delim + rest));
                return output;
            }
        }

        token = wcstok_s(nullptr, &delim, &token_next);

    } while (token != nullptr && maxTokens != 0);

    return output;
}
#pragma optimize("", on)
