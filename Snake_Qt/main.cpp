#include <QtWidgets/QApplication>
#include "Director.h"
#include "Config.h"

int main(int argc, char *argv[])
{
#ifdef NDEBUG
	QApplication::addLibraryPath("./plugins"); //发布版本从程序所在目录下寻找相关组件
#endif // NDEBUG

	QApplication a(argc, argv);
	//初始化相关设置
	Config::getInstance()->init();
	//设定游戏窗口
	Director::getInstance()->setWindow(new Window());
	Director::getInstance()->getWindow()->show();
	a.exec();
	//保存相关设置
	Config::getInstance()->uninit();

	return 0;
}
