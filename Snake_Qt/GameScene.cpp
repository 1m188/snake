#include "GameScene.h"
#include "Director.h"
#include "Config.h"
#include "StartScene.h"
#include "QPainter"
#include "QTimer"
#include "QKeyEvent"
#include "QMessageBox"
#include "QInputDialog"

GameScene::GameScene(Window *parent) :Scene(parent)
{

}

GameScene::~GameScene()
{
}

void GameScene::init(int difficulty)
{
	//初始化游戏数据参数等
	//初始化地图线条数目及其间隔
	lineNum = 25;
	interval = static_cast<double>(width()) / (lineNum + 1);

	//初始化游戏帧数
	fps = 1000 / (200 - difficulty * 50);

	//初始化蛇
	snake = Snake({ 2,2 }, 2, 3);
	//初始化食物
	food.setBodyGrowth(difficulty + 1);
	food.updatePos(0, lineNum, 0, lineNum, snake.getPos());

	//初始化当前分数
	nowScore = 0;
	//初始化分数增加数目
	scoreGrowth = difficulty + 1;

	//设置背景
	setAutoFillBackground(true);
	setPalette(QColor(255, 255, 255));

	//初始化画面更新定时器并开始更新界面
	paintTimer = new QTimer(this);
	connect(paintTimer, &QTimer::timeout, this, &GameScene::updateSnake);
	paintTimer->setInterval(1000 / fps);
	paintTimer->start();
}

void GameScene::updateSnake()
{
	//蛇移动
	snake.move();

	//判定是否吃到食物
	if (snake.eatFood(food))
	{
		//分数刷新
		nowScore += scoreGrowth;

		//更新食物坐标
		food.updatePos(0, lineNum, 0, lineNum, snake.getPos());
	}

	//判定死亡
	if (snake.isDead(0, lineNum, 0, lineNum))
	{
		//停止更新画面
		paintTimer->stop();

		//判定分数是否为最高分
		if (nowScore > Config::getInstance()->getHighestScore())
		{
			//把最高分和最高分保持者保存进Config类中
			Config::getInstance()->setHighestScore(nowScore);
			Config::getInstance()->setHighestScorePlayer(QInputDialog::getText(this, tr(u8"最高分"), tr(u8"恭喜这位少侠！你的分数是：%1，超过了以往所有的记录，为最高分！请在这里记录下你的大名。").arg(nowScore)));
		}

		//是否再来一局
		if (QMessageBox::information(this, tr(u8"游戏结束"), tr(u8"少年，你挂了！你的最终分数是：%1，是否要按照当前速度再来一局？").arg(nowScore), QMessageBox::Yes, QMessageBox::No) == QMessageBox::No)
		{
			StartScene *startScene = new StartScene(Director::getInstance()->getWindow());
			Director::getInstance()->setNowScene(startScene);
			startScene->init();
			startScene->show();
			deleteLater();
			return;
		}
		//再来一局则重新初始化游戏数据等内容
		//初始化分数
		nowScore = 0;

		//初始蛇
		snake = Snake({ 2,2 }, 2, 3);

		//更新食物坐标
		food.updatePos(0, lineNum, 0, lineNum, snake.getPos());

		//开始更新画面
		paintTimer->setInterval(1000 / fps);
		paintTimer->start();

		return;
	}

	//绘制画面
	update();
}

void GameScene::paintEvent(QPaintEvent *event)
{
	QPainter *painter = new  QPainter(this);

	//绘制背景线
	for (int i = 1; i <= lineNum; i++)
	{
		painter->drawLine(0, i*interval, width(), i*interval);
		painter->drawLine(i*interval, 0, i*interval, height());
	}

	//绘制蛇身
	painter->setPen(QPen(Qt::blue));
	painter->setBrush(QBrush(Qt::blue));
	for (QPair<int, int> pos : snake.getPos())
	{
		painter->drawRect(pos.first*interval, pos.second*interval, interval, interval);
	}

	//绘制食物
	painter->setPen(QPen(Qt::red));
	painter->setBrush(QBrush(Qt::red));
	painter->drawRect(food.getPos().first*interval, food.getPos().second*interval, interval, interval);

	//绘制分数
	painter->setFont(QFont(u8"微软雅黑", 12));
	painter->drawText(0, 0, width(), 30, Qt::AlignCenter, QString(tr(u8"分数：%1").arg(nowScore)));

	painter->end();
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
	//按键控制
	//方向控制
	auto snakePos = snake.getPos();
	if (event->key() == Qt::Key_Up && snakePos[0].second - snakePos[1].second != 1)
	{
		snake.setNowDir(0);
	}
	else if (event->key() == Qt::Key_Down && snakePos[0].second - snakePos[1].second != -1)
	{
		snake.setNowDir(1);
	}
	else if (event->key() == Qt::Key_Left && snakePos[0].first - snakePos[1].first != 1)
	{
		snake.setNowDir(2);
	}
	else if (event->key() == Qt::Key_Right && snakePos[0].first - snakePos[1].first != -1)
	{
		snake.setNowDir(3);
	}
	//按住空格加速
	else if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
	{
		paintTimer->setInterval(paintTimer->interval() / 3);
	}
}

void GameScene::keyReleaseEvent(QKeyEvent * event)
{
	//释放空格回到原来的速度
	if (event->key() == Qt::Key_Space && !event->isAutoRepeat())
	{
		paintTimer->setInterval(paintTimer->interval() * 3);
	}
}