#include "stdbool.h"
#include "console.h"
#include "scene/start_scene.h"
#include "scene/game_scene.h"
#include "scene/gameover_scene.h"

int main(int argc, char *argv[])
{
    initConsole();

    bool flag = true;
    while (flag)
    {
        int result = startScene();

        switch (result)
        {
            // 新游戏
        case '1':
            gameoverScene(gameScene());
            break;
            //最高分
        case '2':
            break;
            //退出
        case '3':
            flag = false;
            break;
        }
    }

    return 0;
}