#include "Window.h"
#include "Director.h"
#include "StartScene.h"
#include "QApplication"
#include "QDeskTopWidget"

Window::Window() : QWidget(Q_NULLPTR)
{
	//基本设置
	setAttribute(Qt::WA_DeleteOnClose, true);
	setAttribute(Qt::WA_QuitOnClose, true);

	//设置窗口标题和大小
	setWindowTitle(tr(u8"贪吃蛇"));
	setFixedSize(950, 950);

	//移动窗口到屏幕中央
	QRect rect = frameGeometry();
	rect.moveCenter(QApplication::desktop()->availableGeometry().center());
	move(rect.topLeft());
}

Window::~Window()
{
}

void Window::show()
{
	StartScene *startScene = new StartScene(this);
	Director::getInstance()->setNowScene(startScene);
	startScene->init();
	startScene->show();
	QWidget::show();
}

void Window::keyPressEvent(QKeyEvent * event)
{
	Director::getInstance()->getNowScene()->keyPressEvent(event);
}

void Window::keyReleaseEvent(QKeyEvent * event)
{
	Director::getInstance()->getNowScene()->keyReleaseEvent(event);
}