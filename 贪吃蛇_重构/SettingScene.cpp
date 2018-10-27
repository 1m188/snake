#include "SettingScene.h"
#include "Console.h"
#include "conio.h"

SettingScene::SettingScene()
{
}

SettingScene::~SettingScene()
{
}

void SettingScene::init()
{
	Console *console = Console::getInstance();
	//标题和选项
	console->moveCursor(console->getWindowWidth() / 2 - 4, console->getWindowHeight() / 4);
	printf("游戏设置");
	console->moveCursor(console->getWindowWidth() / 2 - 5, console->getWindowHeight() / 4 + 4);
	printf("1. 前景色");
	console->moveCursor(console->getWindowWidth() / 2 - 5, console->getWindowHeight() / 4 + 8);
	printf("2. 背景色");
	//注意
	console->moveCursor(console->getWindowWidth() / 2 - 15, console->getWindowHeight() / 4 + 12);
	printf("注意：");
	console->moveCursor(console->getWindowWidth() / 2 - 15, console->getWindowHeight() / 4 + 14);
	printf("前背景色不能够设置为同一个颜色");
	console->moveCursor(console->getWindowWidth() / 2 - 15, console->getWindowHeight() / 4 + 16);
	printf("当其设置为同一个颜色的时候");
	console->moveCursor(console->getWindowWidth() / 2 - 15, console->getWindowHeight() / 4 + 18);
	printf("相应颜色变化将在下一次前或背景色设置为不同颜色的时候生效");
}

int SettingScene::run()
{
	//当选择了合适的按键选择之后就返回
	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();
			if (key == '1' || key == '2')
			{
				return key;
			}
		}
		Sleep(1);
	}
}