#include "Console.h"
#include "Config.h"
#include "StartScene.h"
#include "DifficultyChooseScene.h"
#include "GameScene.h"
#include "GameOverScene.h"
#include "HighestScoreScene.h"
#include "SettingScene.h"
#include "ColorChooseScene.h"

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

	//获取游戏相关设置数据的单例
	Config *config = Config::getInstance();
	//初始化加载游戏相关设置数据
	config->init();

	//进入游戏循环
	while (true)
	{
		//游戏开始菜单场景
		StartScene startScene;
		//开始菜单场景初始化
		startScene.init();
		//获取开始菜单场景运行按键返回
		int startSceneRunResult = startScene.run();
		console->clearScreen();

		//如果按下去的键为1，则进入新游戏
		if (startSceneRunResult == '1')
		{
			//难度选择场景
			DifficultyChooseScene difficultyChooseScene;
			difficultyChooseScene.init();
			int difficulty = difficultyChooseScene.run();
			console->clearScreen();
			if (difficulty != 3)
			{
				//游戏主场景
				GameScene gameScene;
				gameScene.init(difficulty); //初始化游戏主场景
				int score = gameScene.run(); //运行游戏主场景
				console->clearScreen();
				//游戏结束场景
				GameOverScene gameOverScene;
				gameOverScene.init(score); //初始化游戏结束场景
				gameOverScene.run(); //运行游戏结束场景
				console->clearScreen();
			}
			else
			{
				//游戏结束场景
				GameOverScene gameOverScene;
				gameOverScene.init(0); //初始化游戏结束场景
				gameOverScene.run(); //运行游戏结束场景
				console->clearScreen();
			}
		}
		//如果按下去的键为2，则显示最高分
		else if (startSceneRunResult == '2')
		{
			//最高分场景
			HighestScoreScene highestScoreScene;
			highestScoreScene.init();
			highestScoreScene.run();
			console->clearScreen();
		}
		//如果按下去的键位3，则进入设置场景
		else if (startSceneRunResult == '3')
		{
			//设置菜单场景
			SettingScene settingScene;
			settingScene.init();
			int settingKey = settingScene.run();
			console->clearScreen();
			//前、背景色选择场景
			ColorChooseScene colorChooseScene(settingKey);
			colorChooseScene.init();
			colorChooseScene.run();
			console->clearScreen();
		}
		//如果按下去的键为4，则退出游戏
		else if (startSceneRunResult == '4')
		{
			break;
		}
	}

	//重新处理好那些加载出来的数据
	config->end();

	return 0;
}