#include "stdio.h"
#include "stdbool.h"
#include "console.h"
#include "utility.h"
#include "start_scene.h"
#include "game_scene.h"
#include "gameover_scene.h"
#include "highestscore_scene.h"
#include "playmode_scene.h"

int main(int argc, char *argv[])
{
    // 读入最高分
    FILE *f = fopen(HIGHEST_SCORE_INFO.highestScoreFileName, "r");
    if (!f)
    {
        f = fopen(HIGHEST_SCORE_INFO.highestScoreFileName, "w");
    }
    else
    {
        fscanf(f, "%d", &HIGHEST_SCORE_INFO.highestScore);
    }
    fclose(f);

    // 初始化控制台信息
    initConsole();

    // 进入主循环流程
    bool flag = true;
    while (flag)
    {
        int result = startScene();

        switch (result)
        {
            // 新游戏
        case '1':
            result = playmodeScene();
            if (result != '3')
            {
                gameoverScene(gameScene(result));
            }
            break;
            //最高分
        case '2':
            highestscoreScene();
            break;
            //退出
        case '3':
            // 写入最高分
            FILE *f = fopen(HIGHEST_SCORE_INFO.highestScoreFileName, "w");
            fprintf(f, "%d", HIGHEST_SCORE_INFO.highestScore);
            fclose(f);
            flag = false;
            break;
        }
    }

    return 0;
}