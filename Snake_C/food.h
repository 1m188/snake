#pragma once

#include "stdbool.h"
#include "snake.h"

// 食物
typedef struct Food
{
    char c;  // 显示字符
    Pos pos; // 坐标
} Food;

// 生成食物
void initFood(Food *food, char c);

// 设置食物的随机坐标，传入坐标的边界
void setRandomFoodPos(Food *food, int xLimit, int yLimit);

// 判定蛇是否吃到食物
bool isGetFood(Snake *snake, Food *food);