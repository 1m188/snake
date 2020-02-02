#include "Snake.h"

Snake::Snake() :Snake({}, 0, 3)
{

}

Snake::Snake(QPair<int, int> head, int sectionNum, int nowDir) : pos({}), dirChange({ { 0,-1 },{ 0,1 },{ -1,0 },{ 1,0 } }), nowDir(Dir::right)
{
	//确定当前方向
	nowDir = (nowDir >= 0 ? nowDir : -nowDir) % 4;
	this->nowDir = static_cast<Dir>(nowDir);

	//初始化蛇身坐标
	pos.append(head);
	for (int i = 0; i < sectionNum - 1; i++)
	{
		QPair<int, int> last = pos.last();
		last.first -= dirChange[this->nowDir].first;
		last.second -= dirChange[this->nowDir].second;
		pos.append(last);
	}
}

Snake::~Snake()
{

}

const QVector<QPair<int, int>> Snake::getPos() const
{
	return pos;
}

void Snake::move()
{
	pos.pop_back(); //推出最后一个
	QPair<int, int> newHead(pos.at(0)); //取头用来构造新头
	//把相应的xy的变化加到新头上
	newHead.first += dirChange[nowDir].first;
	newHead.second += dirChange[nowDir].second;
	//把新头从头部压入蛇身坐标数组，作为真正的新头
	pos.push_front(newHead);
}

bool Snake::isDead(int up, int down, int left, int right)
{
	//判定是否撞到墙
	if (pos[0].first<left || pos[0].first>right || pos[0].second<up || pos[0].second>down)
	{
		return true;
	}
	//否则判定是否自身相撞
	else
	{
		for (int i = 1; i < pos.size(); i++)
		{
			if (pos[0].first == pos[i].first&&pos[0].second == pos[i].second)
			{
				return true;
			}
		}
		return false;
	}
}

void Snake::setNowDir(int nowDir)
{
	this->nowDir = static_cast<Dir>(nowDir % 4);
}

bool Snake::eatFood(Food food)
{
	if (pos[0].first == food.getPos().first&&pos[0].second == food.getPos().second)
	{
		//吃一次食物增长相应的节数
		for (int i = 0; i < food.getBodyGrowth(); i++)
		{
			QPair<int, int> last = pos[pos.size() - 1]; //当前最后一节
			QPair<int, int> secondLast = pos[pos.size() - 2]; //当前倒数第二节
			QPair<int, int> newLast = last; //新的最后一节
			newLast.first += last.first - secondLast.first;
			newLast.second += last.second - secondLast.second;
			pos.append(newLast);
		}
		return true;
	}
	else
	{
		return false;
	}
}