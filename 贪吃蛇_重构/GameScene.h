#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Snake.h"
#include "Food.h"
#include "Scene.h"

//游戏主场景
class GameScene :public Scene
{
private:
	Snake snake; //蛇
	Food food; //食物
	int score; //得分
	int timeout; //每一帧画面显示的延迟时间
	bool isAcc; //是否正在加速

public:
	GameScene();
	~GameScene();

	void init(int timeout); //隐藏基类同名函数，场景初始化
	int run() override; //场景运行
};

#endif // !GAMESCENE_H
