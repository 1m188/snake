#include "console.h"

void displayCursor(HANDLE handle, bool isDisplay)
{
    CONSOLE_CURSOR_INFO info;
    info.bVisible = isDisplay;
    info.dwSize = sizeof(info);
    SetConsoleCursorInfo(handle, &info);
}

void setConsoleTitle(const char *title)
{
    SetConsoleTitle(title);
}

void initConsole()
{
    setConsoleTitle("Snake");
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(h, &info);
    COORD c = {info.srWindow.Right + 1, info.srWindow.Bottom + 1};
    SetConsoleScreenBufferSize(h, c);
}

void setCursorPos(HANDLE handle, COORD *coord)
{
    SetConsoleCursorPosition(handle, *coord);
}