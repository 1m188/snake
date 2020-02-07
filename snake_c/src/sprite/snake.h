#pragma once

#include "Windows.h"
#include "stdbool.h"

// 方向定义
#define UP 1
#define DOWN 2
#define LEFT 3
#define RIGHT 4

// 蛇
typedef struct Snake
{
    char headC;     // 蛇头符号
    char bodyC;     // 蛇身符号
    COORD *pos;     // 蛇身坐标
    int currentDir; // 蛇当前方向
    int len;        // 蛇身长度
} Snake;

// 生成蛇
// 传入蛇身长度，头部符号，身体符号，初始方向，初始头部坐标
void initSnake(Snake *const snake, const int bodyNum, const char headC, const char bodyC, const int initDir, const COORD *const headPos);

// 蛇移动
// input args: snake struct, play mode, the left, right, top, and bottom border array of the map
void move(Snake *const snake, const int mode, const int *const mapBorder);

// 判定蛇是否死亡
// input args: snake struct, play mode, the left, right, top, and bottom border array of the map
// return: if the snake has been dead
const bool isDead(const Snake *const snake, const int mode, const int *const mapBorder);