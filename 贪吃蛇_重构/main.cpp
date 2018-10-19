#include "console.h"
#include "GameScene.h"
#include "GameOverScene.h"

int main()
{
	//获取操作控制台单例
	Console *console = Console::getInstance();
	//设置控制台大小
	console->setWindowSize(100, 50);
	//隐藏光标显示
	console->showCursor(false);

	//游戏主场景
	GameScene gameScene = GameScene();
	//游戏结束场景
	GameOverScene gameOverScene = GameOverScene();
	while (true)
	{
		gameScene.init();
		gameScene.run();
		Console::getInstance()->clearScreen();
		gameOverScene.run();
		Console::getInstance()->clearScreen();
	}

	return 0;
}