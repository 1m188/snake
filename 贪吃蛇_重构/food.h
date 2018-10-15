#ifndef FOOD_H
#define FOOD_H

//食物
class Food
{
private:
	//食物的当前坐标
	int x;
	int y;

public:
	Food();
	~Food();

	void display(bool isDisplay); //显示（或隐藏）
	void updatePos(); //更新食物坐标
	//获取当前食物坐标
	const int getX() const { return x; }
	const int getY() const { return y; }
};

#endif // !FOOD_H
