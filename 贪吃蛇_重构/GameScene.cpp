#include "GameScene.h"
#include "Console.h"
#include "conio.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::init(int timeout)
{
	//实例化相关游戏元素
	snake = Snake({ 11,10 }, 2, 3);
	food = Food();
	score = 0;
	isAcc = false;
	this->timeout = timeout;

	Console *console = Console::getInstance();

	//地图上边界
	console->moveCursor(3, 2);
	for (int i = 2; i < console->getWindowWidth() - 4; i++)
	{
		printf("_");
	}
	//地图下边界
	console->moveCursor(3, console->getWindowHeight() - 2);
	for (int i = 2; i < console->getWindowWidth() - 4; i++)
	{
		printf("~");
	}
	//地图左边界
	for (int i = 3; i < console->getWindowHeight() - 2; i++)
	{
		console->moveCursor(2, i);
		printf("|");
	}
	//地图右边界
	for (int i = 3; i < console->getWindowHeight() - 2; i++)
	{
		console->moveCursor(console->getWindowWidth() - 3, i);
		printf("|");
	}

	//得分信息
	console->moveCursor(5, 1);
	printf("得分：%d", score);
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
	food.display(true); //显示食物

	//进入游戏循环
	while (true)
	{
		snake.display(true); //显示蛇身
		Sleep(timeout); //延迟显示
		snake.display(false); //隐藏蛇身
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
			break;
		}
		//判断是否吃到食物
		if (snake.isEatFood(food))
		{
			food.updatePos(up, down, left, right); //更新食物坐标
			food.display(true);
			//更新得分情况
			score++;
			console->moveCursor(5, 1);
			printf("得分：%d", score);
		}
	}
	return score;
}