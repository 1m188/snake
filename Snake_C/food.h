#pragma once

#include "Windows.h"
#include "stdbool.h"
#include "snake.h"

// 食物
typedef struct Food
{
    char c;  // 显示字符
    COORD pos; // 坐标
} Food;

// 生成食物
void initFood(Food *const food, const char c);

// 设置食物的随机坐标，传入坐标的边界
void setRandomFoodPos(Food *const food, const int xLimit, const int yLimit);

// 判定蛇是否吃到食物
const bool isGetFood(const Snake *const snake, const Food *const food);