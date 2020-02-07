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

    const char *const title = "Play mode choose";

    // choose classic or endless
    const char *text = title;
    COORD c = {width / 2 - strlen(text) / 2, height / 2 - 10};
    setCursorPos(h, &c);
    printf(text);

    text = "1. Classic mode.";
    c.X = width / 2 - strlen(text) / 2, c.Y += 4;
    setCursorPos(h, &c);
    printf(text);

    c.Y += 3;
    setCursorPos(h, &c);
    printf("2. Endless mode.");

    c.Y += 3;
    setCursorPos(h, &c);
    printf("3. Return to main menu.");

    const char *keyList = "123";
    int ch = getKey(keyList, strlen(keyList));
    system("cls");
    if (ch == '3')
    {
        return RETURN_TO_MAIN_MENU_OPT;
    }

    int res = 0;
    // if choose classic mode, the lowest bit of the res will be set 0
    // if choose endless mode, the lowest bit of the res will be set 1
    if (ch == '2')
    {
        res |= 0x01;
    }

    // choose player or ai
    text = title;
    c.X = width / 2 - strlen(text) / 2, c.Y = height / 2 - 10;
    setCursorPos(h, &c);
    printf(text);

    text = "1. Player play mode.";
    c.X = width / 2 - strlen(text) / 2, c.Y += 4;
    setCursorPos(h, &c);
    printf(text);

    c.Y += 3;
    setCursorPos(h, &c);
    printf("2. AI play mode.");

    c.Y += 3;
    setCursorPos(h, &c);
    printf("3. Return to main menu.");

    ch = getKey(keyList, strlen(keyList));
    system("cls");
    if (ch == '3')
    {
        return RETURN_TO_MAIN_MENU_OPT;
    }

    // if choose player mode, the second lowest bit of the res will be set 0
    // if choose ai mode, the second lowest bit of the res will be set 1
    if (ch == '2')
    {
        res |= 0x02;
    }

    return res;
}