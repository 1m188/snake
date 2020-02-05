#include "playmode_scene.h"
#include "stdio.h"
#include "stdint.h"
#include "Windows.h"
#include "console.h"
#include "utility.h"

const int playmodeScene()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    int64_t size = getConsoleSize(handle);
    int32_t height = size;
    int32_t width = size >> 32;

    COORD coord = {width / 2 - 5, height / 2 - 10};
    setCursorPos(handle, &coord);
    printf("Play Mode Choose");

    coord.X -= 2, coord.Y += 4;
    setCursorPos(handle, &coord);
    printf("1. player play.");

    coord.Y += 3;
    setCursorPos(handle, &coord);
    printf("2. AI play.");

    coord.Y += 3;
    setCursorPos(handle, &coord);
    printf("3. return to main menu.");

    char keyList[] = {'1', '2', '3'};
    int len = 3;
    int ch = getKey(keyList, len);
    system("cls");
    return ch;
}