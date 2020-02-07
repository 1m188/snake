#include "gameover_scene.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "Windows.h"
#include "console.h"
#include "utility.h"

void gameoverScene(const int score)
{
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

    int64_t size = getConsoleSize(h);
    int32_t height = size;
    int32_t width = size >> 32;

    // generate the text will be printed
    char text[50];
    memset(text, '\0', 50);
    strcpy(text, "Game over! Your score is: ");
    sprintf(text + strlen(text), "%d", score);

    COORD c = {width / 2 - strlen(text) / 2, height / 2 - 3};
    setCursorPos(h, &c);
    printf("%s", text);

    // 记录最高分
    if (score > HIGHEST_SCORE_INFO.highestScore)
    {
        HIGHEST_SCORE_INFO.highestScore = score;
    }

    getKey(NULL, 0);
    system("cls");
}