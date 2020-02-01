#include "snake.h"
#include "malloc.h"

void initSnake(Snake *snake, int bodyNum, char headC, char bodyC, int initDir, COORD *headPos)
{
    snake->len = bodyNum;

    snake->headC = headC;
    snake->bodyC = bodyC;

    snake->currentDir = initDir;

    snake->pos = (COORD *)malloc(snake->len * sizeof(COORD));
    snake->pos[0].X = headPos->X, snake->pos[0].Y = headPos->Y;
    switch (initDir)
    {
        int i;
    case UP:
        for (i = 1; i < snake->len; i++)
        {
            snake->pos[i].X = snake->pos[i - 1].X;
            snake->pos[i].Y = snake->pos[i - 1].Y + 1;
        }
        break;
    case DOWN:
        for (i = 1; i < snake->len; i++)
        {
            snake->pos[i].X = snake->pos[i - 1].X;
            snake->pos[i].Y = snake->pos[i - 1].Y - 1;
        }
        break;
    case LEFT:
        for (i = 1; i < snake->len; i++)
        {
            snake->pos[i].X = snake->pos[i - 1].X + 1;
            snake->pos[i].Y = snake->pos[i - 1].Y;
        }
        break;
    case RIGHT:
        for (i = 1; i < snake->len; i++)
        {
            snake->pos[i].X = snake->pos[i - 1].X - 1;
            snake->pos[i].Y = snake->pos[i - 1].Y;
        }
        break;
    }
}

void move(Snake *snake)
{
    int i;
    for (i = snake->len - 1; i > 0; i--)
    {
        snake->pos[i].X = snake->pos[i - 1].X;
        snake->pos[i].Y = snake->pos[i - 1].Y;
    }

    switch (snake->currentDir)
    {
    case UP:
        snake->pos[0].Y--;
        break;
    case DOWN:
        snake->pos[0].Y++;
        break;
    case LEFT:
        snake->pos[0].X--;
        break;
    case RIGHT:
        snake->pos[0].X++;
        break;
    }
}

bool isDead(Snake *snake, int leftLimit, int rightLimit, int topLimit, int bottomLimit)
{
    int headX = snake->pos[0].X;
    int headY = snake->pos[0].Y;

    // 判定是否撞到自己
    int i;
    for (i = 4; i < snake->len; i++)
    {
        if (headX == snake->pos[i].X && headY == snake->pos[i].Y)
        {
            return true;
        }
    }

    // 判定是否撞到边界
    return headX <= leftLimit || headX >= rightLimit || headY <= topLimit || headY >= bottomLimit;
}