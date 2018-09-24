#ifndef CONSOLE_H
#define CONSOLE_H

#include "windows.h"
#include "string"
#include "algorithm"

//控制台操纵单例类
class Console final
{
private:
	Console(); //禁止自动构造

	static Console *instance; //唯一实例化指针
	HANDLE stdOut; //控制台句柄

public:
	~Console();

	static Console *getInstance(); //获取单例
	static void deleteInstance(); //删除单例
	void showCursor(bool isShow); //是否显示光标
	void moveCursor(int x, int y); //移动光标
	int getWindowWidth(); //获取控制台窗口宽度
	int getWindowHeight(); //获取控制台窗口高度
	void setWindowSize(int width, int height); //设置控制台窗口大小
};

#endif // !CONSOLE_H

