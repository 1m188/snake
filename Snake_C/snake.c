#include "snake.h"
#include "malloc.h"

Snake initSnake(int bodyNum, char headC, char bodyC, int initDir, Pos *headPos)
{
    Snake snake;

    snake.len = bodyNum;

    snake.headC = headC;
    snake.bodyC = bodyC;

    snake.currentDir = initDir;

    snake.pos = (Pos *)malloc(snake.len * sizeof(Pos));
    snake.pos[0].x = headPos->x, snake.pos[0].y = headPos->y;
    switch (initDir)
    {
        int i;
    case UP:
        for (i = 1; i < snake.len; i++)
        {
            snake.pos[i].x = snake.pos[i - 1].x;
            snake.pos[i].y = snake.pos[i - 1].y + 1;
        }
        break;
    case DOWN:
        for (i = 1; i < snake.len; i++)
        {
            snake.pos[i].x = snake.pos[i - 1].x;
            snake.pos[i].y = snake.pos[i - 1].y - 1;
        }
        break;
    case LEFT:
        for (i = 1; i < snake.len; i++)
        {
            snake.pos[i].x = snake.pos[i - 1].x + 1;
            snake.pos[i].y = snake.pos[i - 1].y;
        }
        break;
    case RIGHT:
        for (i = 1; i < snake.len; i++)
        {
            snake.pos[i].x = snake.pos[i - 1].x - 1;
            snake.pos[i].y = snake.pos[i - 1].y;
        }
        break;
    }

    return snake;
}

void move(Snake *s)
{
    int i;
    for (i = s->len - 1; i > 0; i--)
    {
        s->pos[i].x = s->pos[i - 1].x;
        s->pos[i].y = s->pos[i - 1].y;
    }

    switch (s->currentDir)
    {
    case UP:
        s->pos[0].y--;
        break;
    case DOWN:
        s->pos[0].y++;
        break;
    case LEFT:
        s->pos[0].x--;
        break;
    case RIGHT:
        s->pos[0].x++;
        break;
    }
}

bool isDead(Snake *snake, int leftLimit, int rightLimit, int topLimit, int bottomLimit)
{
    int headX = snake->pos[0].x;
    int headY = snake->pos[0].y;

    // 判定是否撞到自己
    int i;
    for (i = 4; i < snake->len; i++)
    {
        if (headX == snake->pos[i].x && headY == snake->pos[i].y)
        {
            return true;
        }
    }

    // 判定是否撞到边界
    return headX <= leftLimit || headX >= rightLimit || headY <= topLimit || headY >= bottomLimit;
}