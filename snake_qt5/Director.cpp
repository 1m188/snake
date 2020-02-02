#include "Director.h"

Director *Director::instance = nullptr; //初始化类的单例句柄

Director::Director() :window(nullptr), nowScene(nullptr)
{
}

Director::~Director()
{
}

Director * Director::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Director();
	}
	return instance;
}

void Director::setWindow(Window * window)
{
	this->window = window;
}

Window * Director::getWindow()
{
	return window;
}

void Director::setNowScene(Scene * scene)
{
	nowScene = scene;
}

Scene * Director::getNowScene()
{
	return nowScene;
}