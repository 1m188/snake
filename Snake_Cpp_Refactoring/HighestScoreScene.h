#ifndef HIGHESTSCORESCENE_H
#define HIGHESTSCORESCENE_H

#include "Scene.h"

//最高分场景
class HighestScoreScene :public Scene
{
public:
	HighestScoreScene();
	~HighestScoreScene();

	void init() override; //初始化最高分场景
	int run() override; //运行最高分场景
};

#endif // !HIGHESTSCORESCENE_H
