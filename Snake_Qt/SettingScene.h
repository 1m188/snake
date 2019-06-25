#ifndef SETTINGSCENE_H
#define SETTINGSCENE_H

#include "Scene.h"

//设置场景
class SettingScene : public Scene
{
	Q_OBJECT

public:
	SettingScene(Window *parent);
	~SettingScene();

	void init() override; //场景初始化

	private slots:
	void themeChooseButtonClicked(); //单击皮肤选择按钮
	void returnToStartSceneButtonClicked(); //单击返回开始界面按钮
};

#endif // SETTINGSCENE_H
