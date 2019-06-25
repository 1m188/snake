#ifndef DIFFICULTYCHOOSESCENE_H
#define DIFFICULTYCHOOSESCENE_H

#include "Scene.h"

//开始游戏之后的速度选择场景
class DifficultyChooseScene : public Scene
{
	Q_OBJECT

public:
	DifficultyChooseScene(Window *parent);
	~DifficultyChooseScene();

	void init() override; //初始化

	private slots:
	void difficultyChooseButtonClicked(); //单击难度选择按钮
	void returnToStartSceneButtonClicked(); //单击返回开始界面按钮
};

#endif // DIFFICULTYCHOOSESCENE_H
