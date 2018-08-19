#include "snake.h"

Snake::Snake() :pos({ {11,10},{10,10},{9,10} }), moveDir({ {0, -1}, { 0,1 }, { -1,0 }, { 1,0 } }), dir(Direction::right)
{

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
}

void Snake::display(bool isDisplay)
{
	Console *console = Console::getInstance();
	//遍历蛇身坐标然后挨个在相应的位置输出相应的字符
	for (std::list<std::pair<int, int>>::const_iterator it = pos.cbegin(); it != pos.cend(); it++)
	{
		try
		{
			console->moveCursor(it->first, it->second);
		}
		catch (std::exception) //超出控制台大小范围的不画蛇身
		{
			continue;
		}
		if (isDisplay)
		{
			printf("#");
		}
		else
		{
			printf(" ");
		}
	}
}

void Snake::changeDir()
{
	//转换方向
	if (_kbhit())
	{
		switch (_getch())
		{
		case 'w':
		{
			if (dir != Direction::down)
			{
				dir = Direction::up;
			}
			break;
		}
		case 's':
		{
			if (dir != Direction::up)
			{
				dir = Direction::down;
			}
			break;
		}
		case 'a':
		{
			if (dir != Direction::right)
			{
				dir = Direction::left;
			}
			break;
		}
		case 'd':
		{
			if (dir != Direction::left)
			{
				dir = Direction::right;
			}
			break;
		}
		}
	}
}

bool Snake::isDead()
{
	//判定死亡
	return false;
}