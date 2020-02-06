#include "utility.h"
#include "stdio.h"

HighestScoreInfo HIGHEST_SCORE_INFO = {"highestscore.txt", -1};

void readHighestScore(HighestScoreInfo *const highestScoreInfo)
{
    FILE *f = fopen(highestScoreInfo->highestScoreFileName, "r");
    if (!f) // 文件不存在则创建文件
    {
        f = fopen(highestScoreInfo->highestScoreFileName, "w");
    }
    else
    {
        fscanf(f, "%d", &highestScoreInfo->highestScore);
    }
    fclose(f);
}

void writeHighestScore(const HighestScoreInfo *const highestScoreInfo)
{
    FILE *f = fopen(highestScoreInfo->highestScoreFileName, "w");
    fprintf(f, "%d", highestScoreInfo->highestScore);
    fclose(f);
}

const int getKey(const char *const keyList, const unsigned int size)
{
    // 如果列表为NULL或者长度为0
    // 则一旦有按键按下立刻返回
    if (!keyList || !size)
    {
        while (true)
        {
            if (kbhit())
            {
                return getch();
            }
        }
    }
    // 否则需要按下列表中的键
    else
    {
        while (true)
        {
            if (kbhit())
            {
                int ch = getch();
                int i;
                for (i = 0; i < size; i++)
                {
                    if (ch == keyList[i])
                    {
                        return ch;
                    }
                }
            }
        }
    }
}