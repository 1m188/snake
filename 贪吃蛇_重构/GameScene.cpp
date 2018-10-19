#include "GameScene.h"
#include "console.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}

void GameScene::init()
{
	snake = Snake();
	food = Food();
}

int GameScene::run()
{
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
		if (snake.isDead())
		{
			break;
		}
		//判断是否吃到食物
		if (snake.isEatFood(food))
		{
			food.updatePos(); //更新食物坐标
		}
	}
	return 0; //这里构造返回值的原因是为了能够让外部更好的和run函数交互，掌握run函数的情况等等
}