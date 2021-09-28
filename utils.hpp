#pragma once
#include <string>

using std::string;


namespace nsUtils {
    const int
        ERROR_ARGS_COUNT = 1,
        ERROR_FILE = 2,
        ERROR_FORMAT = 3,
        ERROR_OUT_OF_BOUNDS = 4;

    int help();
    void error(int error, const string& msg);


    short to_short(int number);
    short to_short(const string& text, int fromLine);
};
