#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>

//游戏主窗口
class Window : public QWidget
{
	Q_OBJECT

public:
	Window();
	~Window();

private:

public:
	void show(); //覆盖掉原来的show

protected:
	//把相应的按键事件传递到下层场景中去
	void keyPressEvent(QKeyEvent *event) override;
	void keyReleaseEvent(QKeyEvent *event) override;
};

#endif // WINDOW_H
