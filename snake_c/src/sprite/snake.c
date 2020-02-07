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

void move(Snake *const snake, const int mode, const int *const mapBorder)
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
        int top = mapBorder[0], bottom = mapBorder[1], left = mapBorder[2], right = mapBorder[3];
        if (snake->pos[0].X == left)
        {
            snake->pos[0].X = right - 1;
        }
        else if (snake->pos[0].X == right)
        {
            snake->pos[0].X = left + 1;
        }
        else if (snake->pos[0].Y == top)
        {
            snake->pos[0].Y = bottom - 1;
        }
        else if (snake->pos[0].Y == bottom)
        {
            snake->pos[0].Y = top + 1;
        }
    }
}

const bool isDead(const Snake *const snake, const int mode, const int *const mapBorder)
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
    if (mode != ENDLESS_MODE_OPT && (headX <= mapBorder[2] || headX >= mapBorder[3] || headY <= mapBorder[0] || headY >= mapBorder[1]))
    {
        return true;
    }

    return false;
}