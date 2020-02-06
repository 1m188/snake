#include "game_scene.h"
#include "stdbool.h"
#include "stdio.h"
#include "limits.h"
#include "Windows.h"
#include "process.h"
#include "console.h"
#include "snake.h"
#include "food.h"
#include "ql.h"
#include "playmode_scene.h"

const int gameScene(const int mode)
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
    SetConsoleScreenBufferSize(backgroundHandle, consoleInfo.dwSize);
    int width = consoleInfo.dwSize.X, height = consoleInfo.dwSize.Y;
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

    // 分数
    int score = 0;

    // 用作计时函数中的次数增加
    int number = 0;

    // 启动另一个线程用作帧率计时
    _beginthread(timing, 0, &number);

    while (true)
    {
        // when number > 0, excute operations
        if (number <= 0)
        {
            continue;
        }
        else
        {
            number--;
        }

        char ch = '\0';
        if (mode == PLAYER_PLAY_OPT && kbhit()) // 检测按键判定移动方向
        {
            ch = getch();
        }
        else if (mode == AI_PLAY_OPT) // AI play 模式自动玩耍
        {
            ch = qlMove(&snake, &food, -1, width, -1, height);
        }
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

        // 蛇移动
        move(&snake);

        // 判定蛇是否吃到食物
        if (isGetFood(&snake, &food))
        {
            score++;
            setRandomFoodPos(&food, width, height);
            snake.len++;
            snake.pos = (COORD *)realloc(snake.pos, snake.len * sizeof(COORD));
            snake.pos[snake.len - 1].X = snake.pos[snake.len - 2].X + snake.pos[snake.len - 2].X - snake.pos[snake.len - 3].X;
            snake.pos[snake.len - 1].Y = snake.pos[snake.len - 2].Y + snake.pos[snake.len - 2].Y - snake.pos[snake.len - 3].Y;
        }

        // 判定蛇是否死亡
        if (isDead(&snake, -1, width, -1, height))
        {
            number = INT_MIN; // set the number min value to tell the child thread exit
            while (number != INT_MAX) // make sure that child thread has been exited
            {
            };
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
    }

    free(sp);
    sp = NULL;
    free(snake.pos);
    snake.pos = NULL;

    SetConsoleActiveScreenBuffer(mainHandle);
    system("cls");
    return score;
}

void timing(void *num)
{
    int *x = (int *)num;
    int fac = 0;
    while (*x >= 0) // if number >= 0 means that all ok, but if it < 0 means that snake has been dead, and this thread should exit
    {
        Sleep(1);
        fac++;
        if (fac == 60)
        {
            fac = 0;
            (*x)++;
        }
    }
    *x = INT_MAX; // set the number max value to tell the main thread that this thread has been done
}