#include "game_scene.h"
#include "Windows.h"
#include "stdbool.h"
#include "stdio.h"
#include "../console.h"
#include "../snake.h"
#include "../food.h"

void gameScene()
{
    // 双缓冲
    // 前台缓冲区
    const HANDLE mainHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    displayCursor(mainHandle, false);

    // 后台缓冲区
    const HANDLE backgroundHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    displayCursor(backgroundHandle, false);

    // 当前后台缓冲区
    HANDLE backHandle = backgroundHandle;

    COORD coord = {0, 0};
    DWORD b;

    // 设置新创建的缓冲区信息
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(mainHandle, &consoleInfo);
    coord.X = consoleInfo.srWindow.Right, coord.Y = consoleInfo.srWindow.Bottom;
    SetConsoleScreenBufferSize(backgroundHandle, coord);
    int width = coord.X, height = coord.Y;
    int len = width * height;

    // 用来清空缓冲区的缓冲
    char *sp = (char *)malloc(len);
    memset(sp, ' ', len);

    // 初始化蛇和食物
    coord.X = coord.Y = 10;
    Snake snake;
    initSnake(&snake, 2, '#', '+', RIGHT, &coord);
    Food food;
    initFood(&food, '*');
    setRandomFoodPos(&food, width, height);

    while (true)
    {
        // 检测按键判定移动方向
        if (kbhit())
        {
            char ch = getch();
            switch (ch)
            {
            case 'w':
                snake.currentDir = snake.currentDir == DOWN ? snake.currentDir : UP;
                break;
            case 's':
                snake.currentDir = snake.currentDir == UP ? snake.currentDir : DOWN;
                break;
            case 'a':
                snake.currentDir = snake.currentDir == RIGHT ? snake.currentDir : LEFT;
                break;
            case 'd':
                snake.currentDir = snake.currentDir == LEFT ? snake.currentDir : RIGHT;
                break;
            }
        }

        // 蛇移动
        move(&snake);

        // 判定蛇是否吃到食物
        if (isGetFood(&snake, &food))
        {
            setRandomFoodPos(&food, width, height);
            snake.len++;
            snake.pos = (COORD *)realloc(snake.pos, snake.len * sizeof(COORD));
            snake.pos[snake.len - 1].X = snake.pos[snake.len - 2].X + snake.pos[snake.len - 2].X - snake.pos[snake.len - 3].X;
            snake.pos[snake.len - 1].Y = snake.pos[snake.len - 2].Y + snake.pos[snake.len - 2].Y - snake.pos[snake.len - 3].Y;
        }

        // 判定蛇是否死亡
        if (isDead(&snake, -1, width, -1, height))
        {
            break;
        }

        // 清空后台缓冲区
        coord.X = 0, coord.Y = 0;
        WriteConsoleOutputCharacter(backHandle, sp, len, coord, &b);

        // 显示蛇身
        coord.X = snake.pos[0].X, coord.Y = snake.pos[0].Y; // 蛇头
        WriteConsoleOutputCharacter(backHandle, &snake.headC, 1, coord, &b);
        int i;
        for (i = 1; i < snake.len; i++) // 蛇身
        {
            coord.X = snake.pos[i].X, coord.Y = snake.pos[i].Y;
            WriteConsoleOutputCharacter(backHandle, &snake.bodyC, 1, coord, &b);
        }

        // 显示食物
        coord.X = food.pos.X, coord.Y = food.pos.Y;
        WriteConsoleOutputCharacter(backHandle, &food.c, 1, coord, &b);

        // 更换当前缓冲区并且重新确定当前的后台缓冲区
        SetConsoleActiveScreenBuffer(backHandle);
        backHandle = backHandle == backgroundHandle ? mainHandle : backgroundHandle;

        Sleep(100);
    }

    free(sp);
    sp = NULL;
    free(snake.pos);
    snake.pos = NULL;

    SetConsoleActiveScreenBuffer(mainHandle);
    system("cls");
    printf("Game Over!\nPress any key to exit...");
    while (true)
    {
        if (kbhit())
        {
            break;
        }
    }
}