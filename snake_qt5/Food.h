#ifndef FOOD_H
#define FOOD_H

#include "QVector"

//食物类
class Food
{
private:
	//食物坐标
	QPair<int, int> pos;

	//吃到这个食物的时候增长几节
	int bodyGrowth;

public:
	Food();
	~Food();

	//获取食物坐标
	const QPair<int, int> getPos() const;
	//获取增长节数
	const int getBodyGrowth() const;
	//设置增长节数
	void setBodyGrowth(int bodyGrowth);

	//更新食物坐标，传入参数为限制条件
	void updatePos(int up, int down, int left, int right, QVector<QPair<int, int>> snakePos);
};

#endif // FOOD_H
