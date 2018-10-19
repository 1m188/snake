#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "snake.h"
#include "food.h"

//游戏主场景
class GameScene
{
private:
	Snake snake; //蛇
	Food food; //食物

public:
	GameScene();
	~GameScene();

	void init(); //场景初始化
	int run(); //场景运行
};

#endif // !GAMESCENE_H
