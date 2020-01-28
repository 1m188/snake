#pragma once

#include "Windows.h"
#include "stdbool.h"

// 坐标
typedef struct Pos
{
    int x;
    int y;
} Pos;

// 隐藏/显示光标
void displayCursor(HANDLE handle, bool isDisplay);