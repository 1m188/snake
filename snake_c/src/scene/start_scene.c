#include "start_scene.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "Windows.h"
#include "console.h"
#include "utility.h"

const int startScene()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    int64_t size = getConsoleSize(h);
    int32_t height = size;
    int32_t width = size >> 32;

    const char *text = "Snake";
    COORD c = {width / 2 - strlen(text) / 2, height / 2 - 10};
    setCursorPos(h, &c);
    printf(text);

    text = "1. New Game.";
    c.X = width / 2 - strlen(text) / 2, c.Y += 4;
    setCursorPos(h, &c);
    printf(text);

    c.Y += 3;
    setCursorPos(h, &c);
    printf("2. Highest Score.");

    c.Y += 3;
    setCursorPos(h, &c);
    printf("3. exit.");

    char keyList[] = {'1', '2', '3', '\0'};
    int ch = getKey(keyList, strlen(keyList));
    system("cls");
    return ch - '0';
}