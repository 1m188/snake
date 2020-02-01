#pragma once

#include "Windows.h"
#include "stdbool.h"
#include "stdint.h"

// 显示/隐藏光标
void displayCursor(const HANDLE handle, const bool isDisplay);

// 设置控制台标题
void setConsoleTitle(const char *const title);

// 初始化控制台
void initConsole();

// 设置光标坐标
void setCursorPos(const HANDLE handle, const COORD *const coord);

// 获取控制台大小
// 返回64位整数，高32位为宽度，低32位为高度
const int64_t getConsoleSize(const HANDLE handle);