#include "HighestScoreScene.h"
#include "Console.h"
#include "Config.h"
#include "conio.h"

HighestScoreScene::HighestScoreScene()
{
}

HighestScoreScene::~HighestScoreScene()
{
}

void HighestScoreScene::init()
{
	Console *console = Console::getInstance();
	console->moveCursor(console->getWindowWidth() / 2 - 4, console->getWindowHeight() / 4 + 5);
	printf("最高分：%d", Config::getInstance()->highestScore);
	console->moveCursor(console->getWindowWidth() / 2 - 9, console->getWindowHeight() / 4 + 10);
	printf("按任意键返回主菜单...");
}

int HighestScoreScene::run()
{
	//循环检测按键，如果有按键按下的时候就吸收按键并结束这个场景的运行
	while (true)
	{
		if (_kbhit())
		{
			_getch();
			break;
		}
		Sleep(1);
	}
	return Scene::run();
}