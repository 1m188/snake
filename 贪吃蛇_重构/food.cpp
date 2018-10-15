#include "food.h"
#include "console.h"
#include "stdlib.h"
#include "time.h"

Food::Food() :x(0), y(0)
{
	//初始化初始食物坐标
	srand(time(0));
	x = rand() % Console::getInstance()->getWindowWidth();
	srand(time(0));
	y = rand() % Console::getInstance()->getWindowHeight();
}

Food::~Food()
{

}

void Food::display(bool isDisplay)
{
	Console::getInstance()->moveCursor(x, y);
	if (isDisplay)
	{
		printf("*");
	}
	else
	{
		printf(" ");
	}
}

void Food::updatePos()
{
	int newX = 0, newY = 0;
	while (true)
	{
		srand(time(0));
		newX = rand() % Console::getInstance()->getWindowWidth();
		srand(time(0));
		newY = rand() % Console::getInstance()->getWindowHeight();
		if (newX != x && newY != y)
		{
			break;
		}
	}
	x = newX;
	y = newY;
}