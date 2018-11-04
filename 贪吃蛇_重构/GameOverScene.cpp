#include "GameOverScene.h"
#include "Console.h"
#include "Config.h"
#include "conio.h"

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

void GameOverScene::init(int score)
{
	Console *console = Console::getInstance();

	//输出相关信息
	console->moveCursor(console->getWindowWidth() / 2 - 15, console->getWindowHeight() / 4 + 5);
	printf("胜败乃兵家常事，大侠请重新来过");
	console->moveCursor(console->getWindowWidth() / 2 - 6, console->getWindowHeight() / 4 + 8);
	printf("你的得分：%d", score);
	//如果为最高分
	if (score > Config::getInstance()->highestScore)
	{
		printf("  (---恭喜你成为最高分！！！)");
		//更新最高分
		Config::getInstance()->highestScore = score;
	}
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 12);
	printf("按任意键返回主菜单...");
}

int GameOverScene::run()
{
	//让程序暂停而不至于CPU消耗过高，不使用system的原因是这个会输出一行字
	//一旦有按键输入就吸收按键并且跳出循环
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