#pragma once

#include "Windows.h"
#include "stdbool.h"

// 显示/隐藏光标
void displayCursor(HANDLE handle, bool isDisplay);

// 设置控制台标题
void setConsoleTitle(const char *title);

// 初始化控制台
void initConsole();

// 设置光标坐标
void setCursorPos(HANDLE handle, COORD *coord);