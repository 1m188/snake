#pragma once

#include "utility.h"
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
    Pos *pos;       // 蛇身坐标
    int currentDir; // 蛇当前方向
    int len;        // 蛇身长度
} Snake;

// 生成蛇
// 传入蛇身长度，头部符号，身体符号，初始方向，初始头部坐标
Snake initSnake(int bodyNum, char headC, char bodyC, int initDir, Pos *headPos);

// 蛇移动
void move(Snake *s);

// 判定蛇是否死亡
bool isDead(Snake *snake, int leftLimit, int rightLimit, int topLimit, int bottomLimit);