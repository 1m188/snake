#include "GameOverScene.h"
#include "Console.h"
#include "conio.h"

GameOverScene::GameOverScene()
{
}

GameOverScene::~GameOverScene()
{
}

int GameOverScene::run()
{
	//输出相关信息
	Console::getInstance()->moveCursor(0, 0);
	printf("你挂了~");
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