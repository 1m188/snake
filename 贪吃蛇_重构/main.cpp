#include "snake.h"
#include "console.h"

int main()
{
	//获取控制台单例
	Console *console = Console::getInstance();
	//设置控制台大小
	console->setWindowSize(100, 50);
	//隐藏光标显示
	console->showCursor(false);

	//初始化蛇
	Snake snake = Snake();

	//进入游戏循环
	while (true)
	{
		snake.display(true); //显示蛇身
		Sleep(200); //延迟显示
		snake.display(false); //隐藏蛇身
		snake.changeDir(); //改变方向
		snake.move(); //移动
	}

	//删除单例
	console->deleteInstance();

	return 0;
}