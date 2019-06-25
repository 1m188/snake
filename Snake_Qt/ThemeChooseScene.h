#ifndef THEMECHOOSESCENE_H
#define THEMECHOOSESCENE_H

#include "Scene.h"

//主题选择场景
class ThemeChooseScene : public Scene
{
	Q_OBJECT

public:
	ThemeChooseScene(Window *parent);
	~ThemeChooseScene();

	void init() override; //初始化主题选择场景

	private slots:
	void themeChooseButtonClicked(); //单击主题选择按钮
	void returnToStartSceneButtonClicked(); //单击返回开始界面按钮
};

#endif // THEMECHOOSESCENE_H
