#pragma once

#include "snake.h"
#include "food.h"

// snake move auto
// input parameter the snake struct instance, food struct instance, up, down, left, right limits array
// return the key will be pressed
const char autoPressKey(const Snake *const snake, const Food *const food, const int *const mapBorder);

// endless play mode autoPressKey function
// the parm is the same as the autoPressKey
const char autoPressKeyEndless(const Snake *const snake, const Food *const food, const int *const mapBorder);