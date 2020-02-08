#include "gameover_scene.h"
#include "stdio.h"
#include "stdint.h"
#include "string.h"
#include "malloc.h"
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
    char text[100];
    memset(text, '\0', 100);
    strcpy(text, "Game over! Your score is: ");
    sprintf(text + strlen(text), "%d", score);

    COORD c = {width / 2 - strlen(text) / 2, height / 2 - 3};
    setCursorPos(h, &c);
    printf("%s", text);

    // 记录最高分
    if (score > HIGHEST_SCORE_INFO.highestScore)
    {
        HIGHEST_SCORE_INFO.highestScore = score;

        memset(text, '\0', 100);
        strcpy(text, "Congratulations! Your score is a new record! Please print your name: ");
        c.X = width / 2 - strlen(text) / 2, c.Y += 3;
        setCursorPos(h, &c);
        printf("%s", text);

        char name[50];
        memset(name, '\0', 50);
        displayCursor(h, true);
        scanf("%s", name);
        displayCursor(h, false);

        HIGHEST_SCORE_INFO.name = (char *)realloc(HIGHEST_SCORE_INFO.name, (strlen(name) + 1) * sizeof(char));
        memset(HIGHEST_SCORE_INFO.name, '\0', (strlen(name) + 1) * sizeof(char));
        strcpy(HIGHEST_SCORE_INFO.name, name);
    }
    else
    {
        getKey(NULL, 0);
    }

    system("cls");
}