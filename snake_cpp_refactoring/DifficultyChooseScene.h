#ifndef DIFFICULTYCHOOSESCENE_H
#define DIFFICULTYCHOOSESCENE_H

#include "Scene.h"

//新游戏中选择蛇的运行速度场景
class DifficultyChooseScene :public Scene
{
public:
	DifficultyChooseScene();
	~DifficultyChooseScene();

	void init() override; //初始化场景
	int run() override; //运行场景
};

#endif // !DIFFICULTYCHOOSESCENE_H
