#include "Console.h"

//初始化单例模式的获取单例的指针
Console *Console::instance = nullptr;

Console::Console() :stdOut(GetStdHandle(STD_OUTPUT_HANDLE))
{

}

Console & Console::operator=(const Console &)
{
	return *this;
}

Console::~Console()
{

}

Console *Console::getInstance()
{
	//如果单例为空
	if (instance == nullptr)
	{
		//则创造单例
		instance = new Console();
	}
	//最终返回所需单例
	return instance;
}

void Console::showCursor(bool isShow)
{
	//构造一个光标信息结构体
	CONSOLE_CURSOR_INFO info = {};
	//获取当前光标信息
	GetConsoleCursorInfo(stdOut, &info);
	//设置显示光标或隐藏光标
	info.bVisible = isShow;
	//设置光标信息
	SetConsoleCursorInfo(stdOut, &info);
}

void Console::moveCursor(int x, int y)
{
	//当移动到的坐标超过边界就抛出异常
	if (x < 0 || x >= getWindowWidth() || y < 0 || y >= getWindowHeight())
	{
		throw std::exception("args out of range");
	}
	else
	{
		COORD coord = {};
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(stdOut, coord);
	}
}

void Console::clearScreen()
{
	system("cls");
}

void Console::setWindowSize(int width, int height)
{
	//如果要设置的宽度和高度合理
	if (width > 0 && height > 0)
	{
		//这里通过system命令来设置控制台大小，不知道为什么直接设置缓冲区大小不行，，，
		//构建命令字符串
		std::string w = "";
		while (width)
		{
			w += width % 10 + '0';
			width /= 10;
		}
		std::reverse(w.begin(), w.end());

		std::string h = "";
		while (height)
		{
			h += height % 10 + '0';
			height /= 10;
		}
		std::reverse(h.begin(), h.end());

		system(("mode con cols=" + w + " lines=" + h).c_str()); //system命令设置控制台大小
	}
	else //非法参数抛出异常
	{
		throw std::exception("args out of range");
	}
}

int Console::getWindowWidth()
{
	//获取当前控制台缓冲区信息
	CONSOLE_SCREEN_BUFFER_INFO info = {};
	GetConsoleScreenBufferInfo(stdOut, &info);
	return info.dwSize.X; //返回控制台窗口宽度
}

int Console::getWindowHeight()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(stdOut, &info);
	return info.dwSize.Y; //返回控制台窗口高度
}

void Console::setWindowTitle(std::string title)
{
	system((std::string("title ") + title).c_str());
}

std::string Console::getWindowTitle()
{
	char titleTemp[1024]{};
	GetConsoleTitleA(titleTemp, 1024);
	return std::string(titleTemp);
}