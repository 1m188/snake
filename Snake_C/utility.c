#include "utility.h"

void displayCursor(HANDLE handle, bool isDisplay)
{
    CONSOLE_CURSOR_INFO cursor;
    cursor.bVisible = isDisplay;
    cursor.dwSize = sizeof(cursor);
    SetConsoleCursorInfo(handle, &cursor);
}