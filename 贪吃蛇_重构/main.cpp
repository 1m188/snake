#include "snake.h"
#include "console.h"

int main()
{
	Console *console = Console::getInstance();
	console->setWindowSize(100, 50);
	console->showCursor(false);
	Snake snake = Snake();

	while (true)
	{
		snake.display(true);
		Sleep(200);
		snake.display(false);
		snake.changeDir();
		snake.move();
	}
	console->~Console();
	return 0;
}