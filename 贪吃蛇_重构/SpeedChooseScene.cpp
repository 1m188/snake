#include "SpeedChooseScene.h"
#include "Console.h"
#include "conio.h"

SpeedChooseScene::SpeedChooseScene()
{
}

SpeedChooseScene::~SpeedChooseScene()
{
}

void SpeedChooseScene::init()
{
	Console *console = Console::getInstance();
	console->moveCursor(console->getWindowWidth() / 2 - 4, console->getWindowHeight() / 4);
	printf("速度选择");
	console->moveCursor(console->getWindowWidth() / 2 - 4, console->getWindowHeight() / 4 + 5);
	printf("1. 低速");
	console->moveCursor(console->getWindowWidth() / 2 - 4, console->getWindowHeight() / 4 + 10);
	printf("2. 中速");
	console->moveCursor(console->getWindowWidth() / 2 - 4, console->getWindowHeight() / 4 + 15);
	printf("3. 高速");
}

int SpeedChooseScene::run()
{
	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();
			if (key == '1' || key == '2' || key == '3')
			{
				return 400 / (key - '0');
			}
		}
		Sleep(1);
	}
}