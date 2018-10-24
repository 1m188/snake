#include "GameScene.h"
#include "Console.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::init()
{
	//地图、计分等其他
	Console *console = Console::getInstance();
	//地图上边界
	console->moveCursor(2, 2);
	for (int i = 2; i < console->getWindowWidth() - 2; i++)
	{
		printf("_");
	}
	//地图下边界
	console->moveCursor(2, console->getWindowHeight() - 2);
	for (int i = 2; i < console->getWindowWidth() - 2; i++)
	{
		printf("~");
	}
	//地图左边界
	for (int i = 2; i < console->getWindowHeight() - 2; i++)
	{
		console->moveCursor(2, i);
		printf("|");
	}
	//地图右边界
	for (int i = 2; i < console->getWindowHeight() - 2; i++)
	{
		console->moveCursor(console->getWindowWidth() - 3, i);
		printf("|");
	}
	//得分信息
	console->moveCursor(5, 1);
	printf("得分：%d", score);

	//实例化蛇和食物
	snake = Snake();
	food = Food();
	score = 0;
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

	//进入游戏循环
	while (true)
	{
		snake.display(true); //显示蛇身
		food.display(true); //显示食物
		Sleep(200); //延迟显示
		snake.display(false); //隐藏蛇身
		food.display(false); //隐藏食物
		snake.changeDir(); //改变方向
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
			//更新得分情况
			score++;
			console->moveCursor(5, 1);
			printf("得分：%d", score);
		}
	}
	return score;
}