#include "StartScene.h"
#include "Console.h"
#include "conio.h"

StartScene::StartScene()
{
}

StartScene::~StartScene()
{
}

void StartScene::init()
{
	Console *console = Console::getInstance();
	console->moveCursor(console->getWindowWidth() / 2 - 4, console->getWindowHeight() / 4);
	printf("贪 吃 蛇");
	console->moveCursor(console->getWindowWidth() / 2 - 5, console->getWindowHeight() / 4 + 4);
	printf("1. 新游戏");
	console->moveCursor(console->getWindowWidth() / 2 - 5, console->getWindowHeight() / 4 + 8);
	printf("2. 最高分");
	console->moveCursor(console->getWindowWidth() / 2 - 5, console->getWindowHeight() / 4 + 12);
	printf("3. 设置");
	console->moveCursor(console->getWindowWidth() / 2 - 5, console->getWindowHeight() / 4 + 16);
	printf("4. 退出游戏");
}

int StartScene::run()
{
	while (true)
	{
		//如果有按键按下的话
		if (_kbhit())
		{
			//返回按下的按键相对于字符0的大小，即如果按下的是数字的话返回该数字字符的int类型
			return _getch() - '0';
		}
		Sleep(1);
	}
}