#pragma once

#include "snake.h"
#include "food.h"

// snake move with Q-Learning algorithm
// input parameter the snake struct instance, food struct instance, up, down, left, right limits array
// return the key will be pressed
const char qlPressKey(const Snake *const snake, const Food *const food, const int *const mapBorder);

// endless play mode qlPressKey function
// the parm is the same as the qlPressKey
const char qlPressKeyEndless(const Snake *const snake, const Food *const food, const int *const mapBorder);