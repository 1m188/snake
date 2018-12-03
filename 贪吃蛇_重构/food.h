#ifndef FOOD_H
#define FOOD_H

//食物
class Food
{
private:
	//食物的当前坐标
	int x;
	int y;
	//吃了这个食物之后增加多少节
	int section;

public:
	Food();
	~Food();

	void updatePos(int up, int down, int left, int right); //更新食物坐标
	//获取当前食物坐标
	const int getX() const { return x; }
	const int getY() const { return y; }
	//设置吃了该食物之后增加多少节
	void setSection(int section);
	//获取吃了该食物之后增加多少节
	int getSection();
};

#endif // !FOOD_H
