#include "gameover_scene.h"
#include "stdio.h"
#include "stdint.h"
#include "Windows.h"
#include "console.h"
#include "utility.h"

void gameoverScene(const int score)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

    int64_t size = getConsoleSize(handle);
    int32_t height = size;
    int32_t width = size >> 32;

    COORD coord = {width / 2 - 15, height / 2 - 3};
    setCursorPos(handle, &coord);
    printf("Game Over! Your score is: %d", score);
    // 记录最高分
    if (score > HIGHEST_SCORE_INFO.highestScore)
    {
        HIGHEST_SCORE_INFO.highestScore = score;
    }

    getKey(NULL, 0);
    system("cls");
}