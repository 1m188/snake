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
    readHighestScore(&HIGHEST_SCORE_INFO);

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
        case NEWGAME_OPT:
            result = playmodeScene();
            if (result != RETURN_TO_MAIN_MENU_OPT)
            {
                gameoverScene(gameScene(result));
            }
            break;
            //最高分
        case HIGHEST_SCORE_OPT:
            highestscoreScene();
            break;
            //退出
        case EXIT_OPT:
            // 写入最高分
            writeHighestScore(&HIGHEST_SCORE_INFO);
            flag = false;
            break;
        }
    }

    return 0;
}