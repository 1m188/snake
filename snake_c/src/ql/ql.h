#pragma once

#include "snake.h"
#include "food.h"

// snake move with Q-Learning algorithm
// input parameter the snake struct instance, food struct instance, left, right, up, down limits
// return the key will be press
const char qlMove(const Snake *const snake, const Food *const food, const int leftLim, const int rightLim, const int upLim, const int downLim);