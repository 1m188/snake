#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "Scene.h"

//游戏开始菜单场景
class StartScene :public Scene
{
public:
	StartScene();
	~StartScene();

	void init() override; //初始化开始菜单场景
	int run() override; //运行开始菜单场景
};

#endif // !STARTSCENE_H
