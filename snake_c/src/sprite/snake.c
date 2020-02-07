#include "snake.h"
#include "malloc.h"
#include "playmode_scene.h"

void initSnake(Snake *const snake, const int bodyNum, const char headC, const char bodyC, const int initDir, const COORD *const headPos)
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

void move(Snake *const snake, const int mode, const int topLimit, const int bottomLimit, const int leftLimit, const int rightLimit)
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

    // endless mode
    if (mode == ENDLESS_MODE_OPT)
    {
        if (snake->pos[0].X == leftLimit)
        {
            snake->pos[0].X = rightLimit - 1;
        }
        else if (snake->pos[0].X == rightLimit)
        {
            snake->pos[0].X = leftLimit + 1;
        }
        else if (snake->pos[0].Y == topLimit)
        {
            snake->pos[0].Y = bottomLimit - 1;
        }
        else if (snake->pos[0].Y == bottomLimit)
        {
            snake->pos[0].Y = topLimit + 1;
        }
    }
}

const bool isDead(const Snake *const snake, const int mode, const int topLimit, const int bottomLimit, const int leftLimit, const int rightLimit)
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
    // endless mode
    if (mode != ENDLESS_MODE_OPT && (headX <= leftLimit || headX >= rightLimit || headY <= topLimit || headY >= bottomLimit))
    {
        return true;
    }

    return false;
}