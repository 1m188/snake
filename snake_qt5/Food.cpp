#include "Food.h"
#include "QTime"

Food::Food() :pos({}), bodyGrowth(1)
{

}

Food::~Food()
{

}

const QPair<int, int> Food::getPos() const
{
	return pos;
}

const int Food::getBodyGrowth() const
{
	return bodyGrowth;
}

void Food::setBodyGrowth(int bodyGrowth)
{
	this->bodyGrowth = bodyGrowth;
}

void Food::updatePos(int up, int down, int left, int right, QVector<QPair<int, int>> snakePos)
{
	//初始化随机数种子
	qsrand(QTime::currentTime().msec());
again:
	//随机给出新的坐标
	int newX = qrand() % (right - left + 1);
	int newY = qrand() % (down - up + 1);
	//如果和原来的坐标相同则重新计算
	if (newX == pos.first&&newY == pos.second)
	{
		goto again;
	}
	//如果有和蛇身重合的坐标则也重新计算
	for (auto it = snakePos.cbegin(); it < snakePos.cend(); it++)
	{
		if (newX == it->first&&newY == it->second)
		{
			goto again;
		}
	}
	//更新坐标
	pos.first = newX;
	pos.second = newY;
}