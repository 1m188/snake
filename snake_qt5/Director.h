#ifndef DIRECTOR_H
#define DIRECTOR_H

#include "Window.h"
#include "Scene.h"

//导演类
class Director
{
private:
	Director(); //防止自动的外部构造

	static Director *instance; //单例句柄

	Window *window; //游戏窗口指针
	Scene *nowScene; //当前受响应的场景

public:
	~Director();
	Director(const Director &) = delete;
	Director &operator=(const Director &) = delete;

	static Director *getInstance(); //获取单例句柄

	void setWindow(Window *window); //设置游戏窗口指针
	Window *getWindow(); //获取游戏窗口指针
	void setNowScene(Scene *scene); //设置当前受响应场景
	Scene *getNowScene(); //获取当前受响应场景
};

#endif // !DIRECTOR_H
