#include "playmode_scene.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "Windows.h"
#include "console.h"
#include "utility.h"

const int playmodeScene()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    int64_t size = getConsoleSize(h);
    int32_t height = size;
    int32_t width = size >> 32;

    const char *text = "Play Mode Choose";
    COORD c = {width / 2 - strlen(text) / 2, height / 2 - 10};
    setCursorPos(h, &c);
    printf(text);

    text = "1. player play.";
    c.X = width / 2 - strlen(text) / 2, c.Y += 4;
    setCursorPos(h, &c);
    printf(text);

    c.Y += 3;
    setCursorPos(h, &c);
    printf("2. AI play.");

    c.Y += 3;
    setCursorPos(h, &c);
    printf("3. return to main menu.");

    char keyList[] = {'1', '2', '3', '\0'};
    int ch = getKey(keyList, strlen(keyList));
    system("cls");
    return ch - '0';
}