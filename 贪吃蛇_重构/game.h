#ifndef GAME_H
#define GAME_H

#include "windows.h"

class Game
{
private:
	HANDLE stdOut; //控制台句柄
	CONSOLE_CURSOR_INFO cursorInfo; //光标信息结构体

public:
	Game();
	virtual ~Game();

	void showCursor(bool isShow); //是否显示光标
	void moveCursor(int x, int y); //移动光标到x，y处
};

#endif // !GAME_H
