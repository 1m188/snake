#include "food.h"
#include "stdlib.h"
#include "time.h"

void initFood(Food *const food, const char c)
{
    food->c = c;
    food->pos.X = food->pos.Y = 0;
}

void setRandomFoodPos(Food *const food, const int xLimit, const int yLimit)
{
    srand(time(NULL));
    food->pos.X = rand() % xLimit;
    food->pos.Y = rand() % yLimit;
}

const bool isGetFood(const Snake *const snake, const Food *const food)
{
    return snake->pos[0].X == food->pos.X && snake->pos[0].Y == food->pos.Y;
}