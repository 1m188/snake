#ifndef HIGHESTSCORESCENE_H
#define HIGHESTSCORESCENE_H

#include "Scene.h"

class HighestScoreScene : public Scene
{
	Q_OBJECT

public:
	HighestScoreScene(Window *parent);
	~HighestScoreScene();
public:
	void init() override;

	private slots:
	void returnToStartSceneButtonClicked(); //单击返回开始界面按钮
};

#endif // HIGHESTSCORESCENE_H
