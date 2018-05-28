#ifndef SNAKE_H
#define SNAKE_H

#include "game.h"
#include "list"
#include "vector"

class Snake :public Game
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
	void display(char *c); //在蛇身坐标处显示相应的字符（可用作蛇身消除，部分刷新）
};

#endif // !SNAKE_H
