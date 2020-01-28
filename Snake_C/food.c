#include "food.h"
#include "stdlib.h"
#include "time.h"
#include "utility.h"

Food initFood(char c)
{
    Food food;
    food.c = c;
    food.pos.x = food.pos.y = 0;
    return food;
}

void setRandomFoodPos(Food *food, int xLimit, int yLimit)
{
    srand(time(NULL));
    food->pos.x = rand() % xLimit;
    food->pos.y = rand() % yLimit;
}

bool isGetFood(Snake *snake, Food *food)
{
    return snake->pos[0].x == food->pos.x && snake->pos[0].y == food->pos.y;
}