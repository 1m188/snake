#include "Food.h"
#include "Console.h"
#include "stdlib.h"
#include "time.h"

Food::Food() :x(0), y(0), section(1)
{

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

void Food::updatePos(int up, int down, int left, int right)
{
	int newX = 0, newY = 0;
	//生成出现在地图边界之内的坐标
	//直到生成和之前的食物坐标不同的坐标为止
	while (true)
	{
		srand(time(0));
		newX = rand() % (right - left - 1) + left + 1;
		srand(time(0));
		newY = rand() % (down - up - 1) + up + 1;
		if (newX != x && newY != y)
		{
			break;
		}
	}
	x = newX;
	y = newY;
}

void Food::setSection(int section)
{
	this->section = section;
}

int Food::getSection()
{
	return section;
}