#include "GameScene.h"
#include "Console.h"
#include "conio.h"
#include "algorithm"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::init(int difficulty)
{
	//实例化相关游戏元素
	snake = Snake({ 11,10 }, 2, 3);
	food = Food();
	food.setSection(difficulty + 1);
	score = 0;
	timeout = 120 - 40 * difficulty;
	isAcc = false;
	activeScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	backScreenBuffer = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
}

int GameScene::run()
{
	Console *console = Console::getInstance();

	//获取地图边界
	int up = 2;
	int down = console->getWindowHeight() - 2;
	int left = 2;
	int right = console->getWindowWidth() - 3;

	//初始化食物坐标
	food.updatePos(up, down, left, right);

	//这里所用的双缓冲是有两个缓冲区，一个前台一个后台，另外还有一个显示普通界面的
	//控制台屏幕缓冲区，这里第一次渲染的画面采用的是这里类里自己的变量后台缓冲区，
	//之后就是前后台两个缓冲区交替，而原本的控制台屏幕缓冲区则没有设置了。
	//初始化双缓冲所需相关数据
	std::string data;
	COORD coord = {};
	DWORD bytes = 0;

	//进入游戏循环
	while (true)
	{
		//获取判断按键，进行游戏和人的互动
		if (_kbhit())
		{
			int key = _getch();
			//改变蛇的运行方向
			if (key == 'w' || key == 'a' || key == 's' || key == 'd')
			{
				snake.changeDir(key); //改变方向
			}
			//如果按下的是空格，则改变蛇的运行速率
			else if (key == ' ')
			{
				if (isAcc)
				{
					timeout *= 3;
				}
				else
				{
					timeout /= 3;
				}
				isAcc = !isAcc;
			}
		}

		snake.move(); //移动

		//判断死亡
		if (snake.isDead(up, down, left, right))
		{
			//一旦游戏结束，由于两个缓冲区交替代替了原来的缓冲区，因此需要把原来的缓冲区再设置
			//回来，不然那些依靠缓冲区句柄的光标定位什么的就不管用了
			SetConsoleActiveScreenBuffer(console->getStdHandle());
			break;
		}

		//判断是否吃到食物
		if (snake.isEatFood(food))
		{
			food.updatePos(up, down, left, right); //更新食物坐标
			//更新得分情况
			score += food.getSection();
		}

		//显示
		//显示蛇身
		auto snakePos = snake.getPos();
		for (auto &i : snakePos)
		{
			coord.X = i.first;
			coord.Y = i.second;
			WriteConsoleOutputCharacterA(backScreenBuffer, "#", 1, coord, &bytes);
		}

		//显示食物
		coord.X = food.getX();
		coord.Y = food.getY();
		WriteConsoleOutputCharacterA(backScreenBuffer, "*", 1, coord, &bytes);

		//显示分数
		coord.X = 5;
		coord.Y = 1;
		data = "得分：" + std::to_string(score);
		WriteConsoleOutputCharacterA(backScreenBuffer, data.c_str(), data.size(), coord, &bytes);

		//显示地图边界
		//地图上边界
		coord.X = 3;
		coord.Y = 2;
		data.clear();
		for (int i = 2; i < console->getWindowWidth() - 4; i++)
		{
			data.append("_");
		}
		WriteConsoleOutputCharacterA(backScreenBuffer, data.c_str(), data.size(), coord, &bytes);
		//地图下边界
		coord.X = 3;
		coord.Y = console->getWindowHeight() - 2;
		data.clear();
		for (int i = 2; i < console->getWindowWidth() - 4; i++)
		{
			data.append("~");
		}
		WriteConsoleOutputCharacterA(backScreenBuffer, data.c_str(), data.size(), coord, &bytes);
		//地图左边界
		coord.X = 2;
		for (int i = 3; i < console->getWindowHeight() - 2; i++)
		{
			coord.Y = i;
			WriteConsoleOutputCharacterA(backScreenBuffer, "|", 1, coord, &bytes);
		}
		//地图右边界
		coord.X = console->getWindowWidth() - 3;
		for (int i = 3; i < console->getWindowHeight() - 2; i++)
		{
			coord.Y = i;
			WriteConsoleOutputCharacterA(backScreenBuffer, "|", 1, coord, &bytes);
		}

		//双缓冲
		SetConsoleActiveScreenBuffer(backScreenBuffer); //把渲染好的后台缓冲区替换到前台去
		std::swap(activeScreenBuffer, backScreenBuffer); //把前台的缓冲区换成后台缓冲区
		//本来只是想搞一个后台缓冲区，用来和原本的默认前台缓冲区交替做双缓冲，但是不知道为什么，
		//用这个方法的话一旦到了这里清空后台的缓冲区，是能够在前台肉眼看到的，造成了大量的闪烁，
		//仔细想来猜想大概是默认的缓冲区是无论如何都必须在前台，无法替代的，怕导致前台没有缓冲
		//区的矛盾
		//清空后台缓冲区
		data.clear();
		for (int i = 0; i < console->getWindowWidth(); i++)
		{
			data.append(" ");
		}
		coord.X = 0;
		for (int i = 0; i < console->getWindowHeight(); i++)
		{
			coord.Y = i;
			WriteConsoleOutputCharacterA(backScreenBuffer, data.c_str(), data.size(), coord, &bytes);
		}

		Sleep(timeout); //延迟显示
	}
	return score;
}