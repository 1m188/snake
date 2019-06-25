#ifndef STARTSCENE_H
#define STARTSCENE_H

#include "Scene.h"

//开始场景类
class StartScene : public Scene
{
	Q_OBJECT

public:
	StartScene(Window *parent);
	~StartScene();

private:

public:
	void init() override;

	private slots:
	void newGameButtonClicked(); //单击开始新游戏按钮
	void highestScoreButtonClicked(); //单击最高分按钮
	void settingButtonClicked(); //单击设置按钮
};

#endif // STARTSCENE_H
