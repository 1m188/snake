#include "console.h"

void displayCursor(const HANDLE handle, const bool isDisplay)
{
    CONSOLE_CURSOR_INFO info;
    info.bVisible = isDisplay;
    info.dwSize = sizeof(info);
    SetConsoleCursorInfo(handle, &info);
}

void setConsoleTitle(const char *const title)
{
    SetConsoleTitle(title);
}

void initConsole()
{
    setConsoleTitle("Snake");
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    info.dwSize.X = info.srWindow.Right + 1, info.dwSize.Y = info.srWindow.Bottom + 1;
    SetConsoleScreenBufferSize(handle, info.dwSize);
    displayCursor(handle, false);
}

void setCursorPos(const HANDLE handle, const COORD * const coord)
{
    SetConsoleCursorPosition(handle, *coord);
}

const int64_t getConsoleSize(const HANDLE handle)
{
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(handle, &info);
    int32_t width = info.srWindow.Right;
    int32_t height = info.srWindow.Bottom;
    return ((int64_t)width << 32) + height;
}