#include "console.h"

Console *Console::instance = nullptr;

Console::Console()
{
	stdOut = GetStdHandle(STD_OUTPUT_HANDLE);
}

Console::~Console()
{
	delete instance;
	instance = nullptr;
}

Console *Console::getInstance()
{
	if (instance == nullptr)
	{
		instance = new Console();
	}
	return instance;
}

void Console::showCursor(bool isShow)
{
	CONSOLE_CURSOR_INFO info = {};
	GetConsoleCursorInfo(stdOut, &info);
	info.bVisible = isShow;
	SetConsoleCursorInfo(stdOut, &info);
}

void Console::moveCursor(int x, int y)
{
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

int Console::getWindowWidth()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(stdOut, &info);
	return info.dwSize.X;
}

int Console::getWindowHeight()
{
	CONSOLE_SCREEN_BUFFER_INFO info;
	GetConsoleScreenBufferInfo(stdOut, &info);
	return info.dwSize.Y;
}

void Console::setWindowSize(int width, int height)
{
	if (width > 0 && height > 0)
	{
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

		system(("mode con cols=" + w + " lines=" + h).c_str());
	}
	else
	{
		throw std::exception("args out of range");
	}
}