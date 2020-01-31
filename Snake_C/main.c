#include "snake.h"
#include "food.h"
#include "malloc.h"
#include "stdbool.h"
#include "stdio.h"

int main(int argc, char *argv[])
{
    SetConsoleTitle("Snake");

    // 双缓冲
    // 前台缓冲区
    const HANDLE mainHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    displayCursor(mainHandle, false);
    SetConsoleActiveScreenBuffer(mainHandle);

    // 后台缓冲区
    const HANDLE backgroundHandle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    displayCursor(backgroundHandle, false);

    HANDLE backHandle = backgroundHandle;

    // 控制台信息
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(mainHandle, &consoleInfo);
    int width = consoleInfo.dwSize.X, height = consoleInfo.dwSize.Y;
    int len = width * height;

    COORD coord = {0, 0};
    DWORD b;

    char *sp = (char *)malloc(len);
    memset(sp, ' ', len);

    Pos pos = {10, 10};
    Snake snake;
    initSnake(&snake, 2, '#', '+', RIGHT, &pos);
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
            snake.pos = (Pos *)realloc(snake.pos, snake.len * sizeof(Pos));
            snake.pos[snake.len - 1].x = snake.pos[snake.len - 2].x + snake.pos[snake.len - 2].x - snake.pos[snake.len - 3].x;
            snake.pos[snake.len - 1].y = snake.pos[snake.len - 2].y + snake.pos[snake.len - 2].y - snake.pos[snake.len - 3].y;
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
        coord.X = snake.pos[0].x, coord.Y = snake.pos[0].y; // 蛇头
        WriteConsoleOutputCharacter(backHandle, &snake.headC, 1, coord, &b);
        int i;
        for (i = 1; i < snake.len; i++) // 蛇身
        {
            coord.X = snake.pos[i].x, coord.Y = snake.pos[i].y;
            WriteConsoleOutputCharacter(backHandle, &snake.bodyC, 1, coord, &b);
        }

        // 显示食物
        coord.X = food.pos.x, coord.Y = food.pos.y;
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

    SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
    printf("Game Over!\nPress any key to exit...");
    while (true)
    {
        if (kbhit())
        {
            break;
        }
    }

    return 0;
}