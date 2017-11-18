#include "snake.h"
#include "windows.h"
#include "conio.h"

bool gotoxy(int x, int y)
{
	if (x < 0 || y < 0)
	{
		return false;
	}
	else
	{
		HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
		COORD pos;
		pos.X = x;
		pos.Y = y;
		SetConsoleCursorPosition(hOut, pos);
		return true;
	}
}

char JudgeCin()
{
	char ch;
	for (;;)
	{
		if (_kbhit())
		{
			ch = _getch();
			return ch;
		}
	}
}