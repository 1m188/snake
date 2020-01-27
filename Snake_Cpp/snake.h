#pragma once

#include "string"
#include "windows.h"

const HWND hout = FindWindow(LPSTR("ConsoleWindowClass"), NULL);
const CONSOLE_CURSOR_INFO Cursor_info = {1, 0};
const CONSOLE_CURSOR_INFO cursor_info = {1, 1};

const int RightBorder = 120, DownBorder = 30;
const int USERINTERFACE = 1, GAMEINTERFACE = 2;
const int MainMenu = 3, DifficultyChoice = 4;
const int NextChoice = 5, GradeName = 6;
const int MaxGrade = 7, Settings = 8;
const int ColorChoice = 9, LanguageChoice = 10;

enum direction
{
	up = 1,
	down = 2,
	left = 3,
	right = 4
};

class Snake
{
private:
	int x, y;
	Snake *next;
	direction direct;

public:
	Snake();
	Snake(Snake *head);
	~Snake() {}
	friend bool Judgeiflive(Snake *head);
	friend void CreateNext(Snake *head);
	friend void ShowBody(Snake *head);
	friend void ShowAllBody(Snake *head);
	friend void Move(Snake *head);
	friend void ChangeDirect(Snake *head);
	friend void ProductFood(Snake *head);
	friend bool JudgeAboutFood(Snake *head, int a, int b, int gameclass);
};
extern Snake snakehead;
extern int a, b;
extern int grade;
extern int D;
extern int gameclass;
extern Snake *Head;

void PrintMap();
void Time_Wait(int t);
void CGameInterface(int g, std::string name);
void EGameInterface(int g, std::string name);
void CUserInterface(int choice);
void EUserInterface(int choice);

char JudgeCin();
bool gotoxy(int x, int y);
