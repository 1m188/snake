#ifndef SPEEDCHOOSESCENE_H
#define SPEEDCHOOSESCENE_H

#include "Scene.h"

//新游戏中选择蛇的运行速度场景
class SpeedChooseScene :public Scene
{
public:
	SpeedChooseScene();
	~SpeedChooseScene();

	void init() override; //初始化场景
	int run() override; //运行场景
};

#endif // !SPEEDCHOOSESCENE_H
