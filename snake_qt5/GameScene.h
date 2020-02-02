#ifndef GAMESCENE_H
#define GAMESCENE_H

#include "Scene.h"
#include "Snake.h"
#include "Food.h"

//游戏主场景
class GameScene : public Scene
{
	Q_OBJECT

public:
	GameScene(Window *parent);
	~GameScene();

private:
	int lineNum; //地图上要画的线条数目
	double interval; //线条和线条之间的间隔

	//蛇
	Snake snake;
	//食物
	Food food;

	//当前分数
	int nowScore;
	//每次吃到食物的时候分数增加多少
	int scoreGrowth;

	//帧数
	int fps;
	//画面刷新定时器
	QTimer *paintTimer;

public:
	void init(int difficulty); //初始化游戏内容，传入难度系数（同名不同参，隐藏继承的函数）
	void keyPressEvent(QKeyEvent *) override; //重写以根据特定的按下键事件表现不同的行为
	void keyReleaseEvent(QKeyEvent *event) override; //...释放...

protected:
	void paintEvent(QPaintEvent *) override; //重写绘制函数以绘制地图和蛇

	private slots:
	void updateSnake(); //（每一帧）更新画面
};

#endif // GAMESCENE_H
