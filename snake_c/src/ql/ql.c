#include "ql.h"
#include "math.h"
#include "float.h"
#include "Windows.h"

const char qlMove(const Snake *const snake, const Food *const food, const int *const mapBorder)
{
    COORD head = snake->pos[0];

    // action set
    // the order is: up down left right
    COORD actionSet[4] = {{head.X, head.Y - 1}, {head.X, head.Y + 1}, {head.X - 1, head.Y}, {head.X + 1, head.Y}};

    // q table
    // the distance from the point in the action set to the food's pos
    double q[4];
    int i;
    for (i = 0; i < 4; i++)
    {
        q[i] = pow(pow(actionSet[i].X - food->pos.X, 2) + pow(actionSet[i].Y - food->pos.Y, 2), 0.5);
    }

    // if the pos coincides with the snake body or above the border of the map, set it's distance max value
    // if 4 action all set the max value, than the snake will be go die, and than the direction choose will be random
    int j;
    for (i = 0; i < 4; i++)
    {
        COORD c = actionSet[i];
        if (c.X <= mapBorder[2] || c.X >= mapBorder[3] || c.Y <= mapBorder[0] || c.Y >= mapBorder[1])
        {
            q[i] = DBL_MAX;
        }
        else
        {
            for (j = 1; j < snake->len; j++)
            {
                if (c.X == snake->pos[j].X && c.Y == snake->pos[j].Y)
                {
                    q[i] = DBL_MAX;
                    break;
                }
            }
        }
    }

    // choose the min distance value action to go
    j = 0;
    for (i = 1; i < 4; i++)
    {
        if (q[i] < q[j])
        {
            j = i;
        }
    }

    // set direction
    switch (j)
    {
    case 0:
        return 'w';
    case 1:
        return 's';
    case 2:
        return 'a';
    default:
        return 'd';
    }
}