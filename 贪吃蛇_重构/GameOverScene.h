#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "Scene.h"

//游戏结束场景
class GameOverScene :public Scene
{
public:
	GameOverScene();
	~GameOverScene();

	int run() override; //场景运行
};

#endif // !GAMEOVERSCENE_H
