#ifndef SCENE_H
#define SCENE_H

#include <QWidget>
#include "Window.h"

//场景类
class Scene : public QWidget
{
	Q_OBJECT

public:
	Scene(Window *parent);
	virtual ~Scene();

private:

public:
	virtual void init(); //初始化游戏元素和内容
	virtual void keyPressEvent(QKeyEvent *event) override; //把按键事件重写到public下，使得主窗口在分发按键事件的时候能够调用场景的按键响应函数
	virtual void keyReleaseEvent(QKeyEvent *event) override; //同上
};

#endif // SCENE_H
