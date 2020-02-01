#pragma once

#include "Windows.h"
#include "stdbool.h"
#include "stdint.h"

// 显示/隐藏光标
void displayCursor(HANDLE handle, bool isDisplay);

// 设置控制台标题
void setConsoleTitle(const char *title);

// 初始化控制台
void initConsole();

// 设置光标坐标
void setCursorPos(HANDLE handle, COORD *coord);

// 获取控制台大小
// 返回64位整数，高32位为宽度，低32位为高度
int64_t getConsoleSize(HANDLE handle);