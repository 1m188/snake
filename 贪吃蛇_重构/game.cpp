#include "game.h"

Game::Game() :stdOut(GetStdHandle(STD_OUTPUT_HANDLE)), cursorInfo(CONSOLE_CURSOR_INFO())
{
	GetConsoleCursorInfo(stdOut, &cursorInfo);
}

Game::~Game()
{

}

void Game::showCursor(bool isShow)
{
	cursorInfo.bVisible = isShow;
	SetConsoleCursorInfo(stdOut, &cursorInfo);
}

void Game::moveCursor(int x, int y)
{
	COORD cursorPos = COORD();
	cursorPos.X = x;
	cursorPos.Y = y;
	SetConsoleCursorPosition(stdOut, cursorPos);
}