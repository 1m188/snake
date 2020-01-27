#ifndef SNAKE_H
#define SNAKE_H

#include "Food.h"
#include "list"
#include "vector"

//蛇
class Snake
{
private:
	std::list<std::pair<int, int>> pos; //蛇身坐标
	std::vector<std::pair<int, int>> moveDir; //移动的时候的xy变化坐标
	const enum Direction //方向
	{
		up,
		down,
		left,
		right,
	};
	Direction dir; //当前方向（用这个方向索引moveDir可以得到当前的移动的坐标变换）

public:
	Snake();
	//传入初始蛇头坐标、蛇身节数、初始方向来初始化蛇的相关信息
	Snake(std::pair<int, int> headPos, int bodyNum, int dir);
	~Snake();

	void move(); //移动
	const std::list<std::pair<int, int>> getPos()const { return pos; } //获取蛇身坐标列表
	void changeDir(char dir); //转换移动方向
	bool isDead(int up, int down, int left, int right); //判定死亡
	bool isEatFood(Food food); //判定是否吃到食物
};

#endif // !SNAKE_H
