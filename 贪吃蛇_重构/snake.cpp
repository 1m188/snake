#include "snake.h"

Snake::Snake() :pos({ {11,10},{10,10},{9,10} }), moveDir({ {0, -1}, { 0,1 }, { -1,0 }, { 1,0 } }), dir(Direction::right)
{

}

Snake & Snake::operator=(Snake & snake)
{
	this->pos = snake.pos;
	this->dir = snake.dir;
	return *this;
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
	for (auto it = pos.cbegin(); it != pos.cend(); it++)
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
	if (_kbhit()) //如果有按键按下的话
	{
		switch (_getch()) //则获取按键并且判断将要转换的方向
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
	//获取蛇头
	std::pair<int, int> head = pos.front();
	//判断蛇头是否撞到边界
	if (head.first < 0 || head.first >= Console::getInstance()->getWindowWidth() || head.second < 0 || head.second >= Console::getInstance()->getWindowHeight())
	{
		return true;
	}
	//判断蛇头是否撞到自己的身体
	else
	{
		//从蛇头之后第一个开始判断起（如果有的话）
		auto it = pos.cbegin();
		it++;
		for (; it != pos.cend(); it++)
		{
			//如果和蛇头重叠
			if (it->first == head.first&&it->second == head.second)
			{
				return true;
			}
		}
	}
	//否则没死
	return false;
}

bool Snake::isEatFood(Food food)
{
	//如果吃到了
	if (pos.front().first == food.getX() && pos.front().second == food.getY())
	{
		//获取尾巴
		std::pair<int, int> last = pos.back();
		//获取倒数第二节
		pos.pop_back();
		std::pair<int, int> lastSecond = pos.back();
		//重新把尾巴安上去
		pos.push_back(last);
		//新的尾巴
		std::pair<int, int> newLast{};
		//计算新的尾巴的位置
		newLast.first = last.first + last.first - lastSecond.first;
		newLast.second = last.second + last.second - lastSecond.second;
		//安装
		pos.push_back(newLast);
		return true;
	}
	else
	{
		return false;
	}
}