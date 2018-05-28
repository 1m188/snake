#include "snake.h"

Snake::Snake() :Game(), pos({}), moveDir({ {0, -1}, { 0,1 }, { -1,0 }, { 1,0 } }), dir(Direction::right)
{
	pos.push_back({ 11,10 });
	pos.push_back({ 10,10 });
	pos.push_back({ 9,10 });
}

Snake::~Snake()
{

}

void Snake::move()
{
	//移动
	pos.pop_back(); //弹出尾巴
	std::pair<int, int> newHead(pos.front()); //用原来的头初始化新头
	//原来的头按照方向移动就是新头
	newHead.first += moveDir[dir].first;
	newHead.second += moveDir[dir].second;
	//把新头装上去成为真正的新头
	pos.push_front(newHead);

	//转换方向

	//判定死亡
}

void Snake::display(char *c)
{
	//遍历蛇身坐标然后挨个在相应的位置输出相应的字符
	for (std::list<std::pair<int, int>>::const_iterator it = pos.begin(); it != pos.end(); it++)
	{
		moveCursor(it->first, it->second);
		printf(c);
	}
}