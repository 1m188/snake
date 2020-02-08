#include "highestscore_scene.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "Windows.h"
#include "console.h"
#include "utility.h"

void highestscoreScene()
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    int64_t size = getConsoleSize(h);
    int32_t height = size;
    int32_t width = size >> 32;

    char text[100];
    memset(text, '\0', 100);
    if (HIGHEST_SCORE_INFO.highestScore < 0)
    {
        strcpy(text, "There are no highest score. Waiting for you to creating!");
    }
    else
    {
        strcpy(text, "The highest score is: ");
        sprintf(text + strlen(text), "%d", HIGHEST_SCORE_INFO.highestScore);
    }
    COORD c = {width / 2 - strlen(text) / 2, height / 2 - 5};
    setCursorPos(h, &c);
    printf("%s", text);

    getKey(NULL, 0);
    system("cls");
}