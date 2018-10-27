#ifndef SETTINGSCENE_H
#define SETTINGSCENE_H

#include "Scene.h"

//…Ë÷√≥°æ∞
class SettingScene :public Scene
{
public:
	SettingScene();
	~SettingScene();

	void init() override;
	int run() override;
};

#endif // !SETTINGSCENE_H
