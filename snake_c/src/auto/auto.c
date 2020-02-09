#include "auto.h"
#include "math.h"
#include "float.h"
#include "Windows.h"

const char autoPressKey(const Snake *const snake, const Food *const food, const int *const mapBorder)
{
    COORD head = snake->pos[0];

    // action set
    // the order is: up down left right
    COORD actionSet[4] = {{head.X, head.Y - 1}, {head.X, head.Y + 1}, {head.X - 1, head.Y}, {head.X + 1, head.Y}};

    // score table
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

const char autoPressKeyEndless(const Snake *const snake, const Food *const food, const int *const mapBorder)
{
    COORD head = snake->pos[0];
    // action set
    // the order is: up down left right
    COORD actionSet[4] = {{head.X, head.Y - 1}, {head.X, head.Y + 1}, {head.X - 1, head.Y}, {head.X + 1, head.Y}};

    // score table
    // the distance from the point in the action set to the food's pos
    double q[4];
    int i;
    int top = mapBorder[0], bottom = mapBorder[1], left = mapBorder[2], right = mapBorder[3];
    int fx = food->pos.X, fy = food->pos.Y;
    // due to the endless mode, it will calc 3 distance between the head pos and food pos, and don't mind the border,
    // choose the min distance x and y, and set the q table value
    for (i = 0; i < 4; i++)
    {
        int ax = actionSet[i].X, ay = actionSet[i].Y;

        double dx1 = abs(ax - fx);
        double dx2 = (ax - (left + 1)) + (right - 1 - fx);
        double dx3 = (right - 1 - ax) + (fx - (left + 1));
        double dxmin = dx1 < dx2 ? dx1 : dx2;
        if (dx3 < dxmin)
        {
            dxmin = dx3;
        }

        double dy1 = abs(ay - fy);
        double dy2 = (ay - (top + 1)) + (bottom - 1 - fy);
        double dy3 = (bottom - 1 - ay) + (fy - (top + 1));
        double dymin = dy1 < dy2 ? dy1 : dy2;
        if (dy3 < dymin)
        {
            dymin = dy3;
        }

        q[i] = pow(pow(dxmin, 2) + pow(dymin, 2), 0.5);
    }

    // if the pos coincides with the snake body, set it's distance max value
    // if 4 action all set the max value, than the snake will be go die, and than the direction choose will be random
    int j;
    for (i = 0; i < 4; i++)
    {
        for (j = 1; j < snake->len; j++)
        {
            if (actionSet[i].X == snake->pos[j].X && actionSet[i].Y == snake->pos[j].Y)
            {
                q[i] = DBL_MAX;
                break;
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