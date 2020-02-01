#pragma once

#include "stdbool.h"

// 最高分数据记录文件
extern const char *const HIGHEST_SCORE_FILE_NAME;

// 当前最高分
extern int HIGHEST_SCORE;

// 阻塞获取按键，直到按键为keyList中的值为止
// 传入按键列表和其长度
const int getKey(const char *const keyList, const unsigned int size);