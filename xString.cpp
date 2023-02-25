#include <utf8.h>
#include "xString.hpp"

using std::string, std::vector, std::wstring;


string xString::fromWide(const wstring& text) {
    string out;
    utf8::utf16to8(text.begin(), text.end(), back_inserter(out));
    return out;
}
wstring xString::toWide(const string& text) {
    wstring out;
    utf8::utf8to16(text.begin(), text.end(), back_inserter(out));
    return out;
}


#pragma optimize("", off)
vector<string> xString::split(const string& source, const char delim, int maxTokens) {
    vector<string> output;
    auto input(source);

    char* token_next = {};
    auto* token = strtok_s(input.data(), &delim, &token_next);

    do {
        maxTokens--;

        if (token != nullptr) {
            if (maxTokens != 0)
                output.emplace_back(token);

            else if (token_next != nullptr) {
                const auto rest = string(token_next);
                output.emplace_back(string(token) +
                    (rest.empty() ?
                         "" :
                         delim + rest));
                return output;
            }
        }

        token = strtok_s(nullptr, &delim, &token_next);

    } while (token != nullptr && maxTokens != 0);

    return output;
}
#pragma optimize("", on)
