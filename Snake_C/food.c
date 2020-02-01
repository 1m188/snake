#include "food.h"
#include "stdlib.h"
#include "time.h"

void initFood(Food *food, char c)
{
    food->c = c;
    food->pos.X = food->pos.Y = 0;
}

void setRandomFoodPos(Food *food, int xLimit, int yLimit)
{
    srand(time(NULL));
    food->pos.X = rand() % xLimit;
    food->pos.Y = rand() % yLimit;
}

bool isGetFood(Snake *snake, Food *food)
{
    return snake->pos[0].X == food->pos.X && snake->pos[0].Y == food->pos.Y;
}