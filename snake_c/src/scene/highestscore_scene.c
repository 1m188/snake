#include "highestscore_scene.h"
#include "Windows.h"
#include "stdio.h"
#include "stdint.h"
#include "console.h"
#include "utility.h"

void highestscoreScene()
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    int64_t size = getConsoleSize(handle);
    int32_t height = size;
    int32_t width = size >> 32;

    COORD coord;
    if (HIGHEST_SCORE < 0)
    {
        coord.X = width / 2 - 25, coord.Y = height / 2 - 5;
        setCursorPos(handle, &coord);
        printf("There are no highest score. Waiting for you to creating!");
    }
    else
    {
        coord.X = width / 2 - 13, coord.Y = height / 2 - 5;
        setCursorPos(handle, &coord);
        printf("The highest score is: %d", HIGHEST_SCORE);
    }

    getKey(NULL, 0);
    system("cls");
}