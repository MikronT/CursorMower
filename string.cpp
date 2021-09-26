#include "string.hpp"

using namespace nsStringLib;


int nsStringLib::string_getSize(const string& of) { return string_getSize(of.c_str()); }
int nsStringLib::string_getSize(const char* of) {
    auto size = 0;
    while (*of) size += (*of++ & 0xc0) != 0x80;
    return size;
}