#include "start_scene.h"
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"
#include "Windows.h"
#include "../console.h"

int startScene()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    int64_t size = getConsoleSize(handle);
    int32_t height = size;
    int32_t width = size >> 32;

    COORD coord = {width / 2 - 2, height / 2 - 10};
    setCursorPos(handle, &coord);
    printf("Snake");

    coord.X -= 4, coord.Y += 4;
    setCursorPos(handle, &coord);
    printf("1. New Game.");

    coord.Y += 3;
    setCursorPos(handle, &coord);
    printf("2. Highest Score.");

    coord.Y += 3;
    setCursorPos(handle, &coord);
    printf("3. exit.");

    while (true)
    {
        if (kbhit())
        {
            int ch = getch();
            if (ch == '1' || ch == '2' || ch == '3')
            {
                system("cls");
                return ch;
            }
        }
    }
}