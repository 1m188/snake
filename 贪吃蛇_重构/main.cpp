#include "snake.h"
#include "console.h"
#include "food.h"

int main()
{
	//获取操作控制台单例
	Console *console = Console::getInstance();
	//设置控制台大小
	console->setWindowSize(100, 50);
	//隐藏光标显示
	console->showCursor(false);

	//初始化蛇
	Snake snake = Snake();
	//初始化食物
	Food food = Food();

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
			//隐藏游戏元素，输出信息
			snake.display(false);
			food.display(false);
			console->moveCursor(0, 0);
			printf("你挂了~");
			break;
		}
		//判断是否吃到食物
		if (snake.isEatFood(food))
		{
			food.updatePos(); //更新食物坐标
		}
	}

	//删除操作控制台单例
	Console::deleteInstance();

	//让程序暂停而不至于CPU消耗过高，不使用system的原因是这个会输出一行字
	while (1)
	{
		Sleep(1);
	}
	return 0;
}