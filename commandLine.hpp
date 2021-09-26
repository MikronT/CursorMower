#pragma once
#include <Windows.h>


namespace nsCommandLineLib {
    void setConInfo(HANDLE& console_out, CONSOLE_SCREEN_BUFFER_INFOEX& info);

    [[nodiscard]] CONSOLE_SCREEN_BUFFER_INFOEX getConInfo(HANDLE& console_out);
}