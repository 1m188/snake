#pragma once

#include "stdbool.h"

typedef struct HighestScoreInfo
{
    const char *const highestScoreFileName; // 最高分数据记录文件
    int highestScore;                       // 当前最高分
} HighestScoreInfo;
extern HighestScoreInfo HIGHEST_SCORE_INFO; // 记录最高分相关信息结构体实例

void readHighestScore(HighestScoreInfo *const highestScoreInfo);        // 读入最高分
void writeHighestScore(const HighestScoreInfo *const highestScoreInfo); // 写入最高分

// 阻塞获取按键，直到按键为keyList中的值为止
// 传入按键列表和其长度
const int getKey(const char *const keyList, const unsigned int size);