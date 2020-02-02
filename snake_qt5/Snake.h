#ifndef SNAKE_H
#define SNAKE_H

#include "QVector"
#include "Food.h"

//蛇类
class Snake
{
private:
	//蛇身坐标
	QVector<QPair<int, int>> pos;

	//方向
	const enum Dir
	{
		up,
		down,
		left,
		right,
	};
	QVector<QPair<int, int>> dirChange; //沿着方向行进的时候的坐标的变化
	Dir nowDir; //当前蛇头方向

public:
	Snake();
	Snake(QPair<int, int> head, int sectionNum, int nowDir);
	~Snake();

	//获取蛇身坐标
	const QVector<QPair<int, int>> getPos() const;

	//移动
	void move();
	//是否死亡
	bool isDead(int up, int down, int left, int right);
	//设置当前方向
	void setNowDir(int nowDir);
	//吃到食物
	bool eatFood(Food food);
};

#endif // SNAKE_H
