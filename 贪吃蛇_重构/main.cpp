#include "Console.h"
#include "StartScene.h"
#include "GameScene.h"
#include "GameOverScene.h"

int main()
{
	//获取操作控制台单例
	Console *console = Console::getInstance();
	//设置控制台大小
	console->setWindowSize(100, 50);
	//设置控制台标题
	console->setWindowTitle("贪吃蛇");
	//隐藏光标显示
	console->showCursor(false);

	while (true)
	{
		//游戏开始菜单场景
		StartScene startScene;
		//开始菜单场景初始化
		startScene.init();
		//获取开始菜单场景运行按键返回
		int startSceneRunResult = startScene.run();

		//如果按下去的键为1，则进入新游戏
		if (startSceneRunResult == 1)
		{
			console->clearScreen(); //清屏
			//游戏主场景
			GameScene gameScene;
			gameScene.init(); //初始化游戏主场景
			gameScene.run(); //运行游戏主场景
			console->clearScreen();
			//游戏结束场景
			GameOverScene gameOverScene;
			gameOverScene.init(); //初始化游戏结束场景
			gameOverScene.run(); //运行游戏结束场景
			console->clearScreen();
		}
		//如果按下去的键位4，则退出游戏
		else if (startSceneRunResult == 4)
		{
			break;
		}
		//如果按下的不是既定的按键，则清屏，并且重新绘制一次开始菜单场景
		else
		{
			console->clearScreen();
		}
	}

	return 0;
}