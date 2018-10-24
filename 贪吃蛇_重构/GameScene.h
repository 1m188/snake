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

public:
	GameScene();
	~GameScene();

	void init() override; //场景初始化
	int run() override; //场景运行
};

#endif // !GAMESCENE_H
