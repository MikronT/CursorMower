#include "string.hpp"


int nsString::string_getSize(const string& of) { return string_getSize(of.c_str()); }
int nsString::string_getSize(const char* of) {
    auto size = 0;
    while (*of)
        size += (*of++ & 0xc0) != 0x80;
    return size;
}

vector<string> nsString::string_split(vector<char>& source, const char delim, const int maxTokens) {
    return string_split(string(source.begin(), source.end()), delim, maxTokens);
}
#pragma optimize("", off)
vector<string> nsString::string_split(const string& source, const char delim, int maxTokens) {
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
                output.emplace_back(string(token) + (rest.empty() ? "" : delim + rest));
                return output;
            }
        }

        token = strtok_s(nullptr, &delim, &token_next);

    } while (token != nullptr && maxTokens != 0);

    return output;
}
#pragma optimize("", on)
