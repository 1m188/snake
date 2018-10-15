#ifndef SNAKE_H
#define SNAKE_H

#include "console.h"
#include "list"
#include "vector"
#include "conio.h"
#include "food.h"

//蛇
class Snake
{
private:
	std::list<std::pair<int, int>> pos; //蛇身坐标
	const std::vector<std::pair<int, int>> moveDir; //移动的时候的xy变化坐标
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
	~Snake();

	void move(); //移动
	void display(bool isDisplay); //是否显示蛇身（可用作蛇身消除，部分刷新）
	void changeDir(); //转换移动方向
	bool isDead(); //判定死亡
	bool isEatFood(Food food); //判定是否吃到食物
};

#endif // !SNAKE_H
