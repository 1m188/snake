#include "snake.h"
//#include "conio.h" //getch()函数，用于接收玩家的随时输入

int main()
{
	Snake snake = Snake();
	snake.showCursor(false);

	while (true)
	{
		snake.display("#");
		Sleep(800);
		snake.display(" ");
		snake.move();
	}
	return 0;
}