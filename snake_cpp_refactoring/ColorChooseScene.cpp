#include "ColorChooseScene.h"
#include "Console.h"
#include "Config.h"
#include "conio.h"

ColorChooseScene::ColorChooseScene(int mode) :mode(mode)
{
}

ColorChooseScene::~ColorChooseScene()
{
}

void ColorChooseScene::init()
{
	Console *console = Console::getInstance();
	console->moveCursor(console->getWindowWidth() / 2 - 4, console->getWindowHeight() / 4);
	//根据前、背景色选择模式不同输出不同的标题
	if (mode == '1')
	{
		printf("前景色选择");
	}
	else if (mode == '2')
	{
		printf("背景色选择");
	}
	//具体的颜色选项
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 4);
	printf("0. 黑色");
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 7);
	printf("1. 蓝色");
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 10);
	printf("2. 绿色");
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 13);
	printf("3. 湖蓝色");
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 16);
	printf("4. 红色");
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 19);
	printf("5. 紫色");
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 22);
	printf("6. 黄色");
	console->moveCursor(console->getWindowWidth() / 2 - 10, console->getWindowHeight() / 4 + 25);
	printf("7. 白色");
	console->moveCursor(console->getWindowWidth() / 2 + 5, console->getWindowHeight() / 4 + 4);
	printf("8. 灰色");
	console->moveCursor(console->getWindowWidth() / 2 + 5, console->getWindowHeight() / 4 + 7);
	printf("9. 浅蓝色");
	console->moveCursor(console->getWindowWidth() / 2 + 5, console->getWindowHeight() / 4 + 10);
	printf("a. 浅绿色");
	console->moveCursor(console->getWindowWidth() / 2 + 5, console->getWindowHeight() / 4 + 13);
	printf("b. 浅草绿");
	console->moveCursor(console->getWindowWidth() / 2 + 5, console->getWindowHeight() / 4 + 16);
	printf("c. 浅红色");
	console->moveCursor(console->getWindowWidth() / 2 + 5, console->getWindowHeight() / 4 + 19);
	printf("d. 浅紫色");
	console->moveCursor(console->getWindowWidth() / 2 + 5, console->getWindowHeight() / 4 + 22);
	printf("e. 浅黄色");
	console->moveCursor(console->getWindowWidth() / 2 + 5, console->getWindowHeight() / 4 + 25);
	printf("f. 亮白色");
}

int ColorChooseScene::run()
{
	while (true)
	{
		if (_kbhit())
		{
			int key = _getch();
			//判定按键是否为特定按键，如果是的话将其变换成对应数字
			if (key >= '0'&&key <= '9')
			{
				key -= '0';
			}
			else if (key >= 'a'&&key <= 'f')
			{
				key = key - 'a' + 10;
			}
			//如果key经过变换的话，就应符合这里的判断条件，以求一次判断处理
			if (key >= 0 && key <= 15)
			{
				//获取相应的按键变换后的转换为的color类型的变量
				Config::color color = static_cast<Config::color>(key);
				//前景色
				if (mode == '1')
				{
					//更新设置类中的前景色
					Config::getInstance()->foreColor = color;
					//单独重设控制台窗口的前景色
					Console::getInstance()->setWindowColor(color, Console::getInstance()->getWindowBackgroundColor());
				}
				//背景色
				else if (mode == '2')
				{
					//更新设置类中的背景色
					Config::getInstance()->backgroundColor = color;
					//单独重设控制台窗口的背景色
					Console::getInstance()->setWindowColor(Console::getInstance()->getWindowForeColor(), color);
				}
				break;
			}
		}
		Sleep(1);
	}
	return Scene::run();
}