#include "Snake.h"
#include "Console.h"

Snake::Snake() :Snake({ 0,0 }, 0, 0)
{
}

Snake::Snake(std::pair<int, int> headPos, int bodyNum, int dir) : pos({}), moveDir({ {0, -1}, { 0,1 }, { -1,0 }, { 1,0 } }), dir(static_cast<Direction>((dir < 0 ? -dir : dir) % 4))
{
	//按照蛇头坐标、蛇身节数和初始方向初始化蛇身坐标
	pos.push_back(headPos);
	for (int i = 0; i < bodyNum; i++)
	{
		pos.push_back({ pos.back().first - moveDir[this->dir].first, pos.back().second - moveDir[this->dir].second });
	}
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

void Snake::changeDir(char dir)
{
	//转换方向
	//判断传进来的方向字符
	switch (dir)
	{
	case 'w':
	{
		if (this->dir != Direction::down)
		{
			this->dir = Direction::up;
		}
		break;
	}
	case 's':
	{
		if (this->dir != Direction::up)
		{
			this->dir = Direction::down;
		}
		break;
	}
	case 'a':
	{
		if (this->dir != Direction::right)
		{
			this->dir = Direction::left;
		}
		break;
	}
	case 'd':
	{
		if (this->dir != Direction::left)
		{
			this->dir = Direction::right;
		}
		break;
	}
	}
}

bool Snake::isDead(int up, int down, int left, int right)
{
	//获取蛇头
	std::pair<int, int> head = pos.front();
	//判断蛇头是否撞到边界
	if (head.first <= left || head.first >= right || head.second <= up || head.second >= down)
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
		//吃了这个食物之后蛇身增加多少节
		for (int i = 0; i < food.getSection(); i++)
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
		}
		return true;
	}
	else
	{
		return false;
	}
}