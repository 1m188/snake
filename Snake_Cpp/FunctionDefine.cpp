#include "snake.h"
#include "iostream"
#include "cstdlib"
#include "windows.h"
#include "ctime"
#include "conio.h"
#include "string"

int grade;
int a, b;
int D;
int gameclass;
Snake snakehead;
Snake *Head;

Snake::Snake()
{
	next = 0;
	x = 10;
	y = 10;
	direct = right;
}

Snake::Snake(Snake *head)
{
	Snake *p = head;
	next = 0;
	for (; p->next != 0;)
	{
		p = p->next;
	}
	direct = p->direct;
	switch (p->direct)
	{
	case up:
	{
		x = p->x;
		y = p->y + 1;
		break;
	}
	case down:
	{
		x = p->x;
		y = p->y - 1;
		break;
	}
	case left:
	{
		y = p->y;
		x = p->x + 1;
		break;
	}
	case right:
	{
		y = p->y;
		x = p->x - 1;
		break;
	}
	}
}

bool Judgeiflive(Snake *head)
{
	Snake *p = head;
	if (head->x == 0 || head->x == RightBorder - 1 || head->y == 0 || head->y == DownBorder - 1)
	{
		return false;
	}
	else
	{
		p = p->next;
		for (; p != 0;)
		{
			if (p->x == head->x && p->y == head->y)
			{
				return false;
			}
			else
			{
				p = p->next;
			}
		}
		return true;
	}
}

void CreateNext(Snake *head)
{
	Snake *p = head;
	for (;;)
	{
		if (p->next == 0)
		{
			break;
		}
		p = p->next;
	}
	Snake *s = new Snake(Head);
	p->next = s;
}

void ShowBody(Snake *head)
{
	Snake *p = head;
	gotoxy(p->x, p->y);
	std::cout << '#';
	for (; p->next != 0;)
	{
		p = p->next;
	}
}

void ShowAllBody(Snake *head)
{
	Snake *p = head;
	for (; p != 0;)
	{
		if ((p->x <= 0 || p->x >= RightBorder - 1) || (p->y <= 0 || p->y >= DownBorder - 1))
		{
			break;
		}
		gotoxy(p->x, p->y);
		std::cout << '#';
		p = p->next;
	}
}

void Move(Snake *head)
{
	Snake *p = head;
	Snake *ps = head;
	char ch;
	if (_kbhit())
	{
		ch = _getch();
		switch (ch)
		{
		case 'w':
		case 'W':
		{
			if (p->direct == up || p->direct == down)
			{
				goto move;
			}
			else
			{
				goto caseup;
			}
			break;
		}
		case 's':
		case 'S':
		{
			if (p->direct == up || p->direct == down)
			{
				goto move;
			}
			else
			{
				goto casedown;
			}
			break;
		}
		case 'a':
		case 'A':
		{
			if (p->direct == left || p->direct == right)
			{
				goto move;
			}
			else
			{
				goto caseleft;
			}
			break;
		}
		case 'd':
		case 'D':
		{
			if (p->direct == left || p->direct == right)
			{
				goto move;
			}
			else
			{
				goto caseright;
			}
			break;
		}
		default:
		{
			goto move;
		}
		}
	}
	else
	{
	move:
		switch (p->direct)
		{
		case up:
		{
		caseup:
			for (; ps->next != 0;)
			{
				ps = ps->next;
			}
			gotoxy(ps->x, ps->y);
			std::cout << ' ';
			ps->x = head->x;
			ps->y = head->y - 1;
			Head = ps;
			Head->direct = up;
			for (; p->next->next != 0;)
			{
				p = p->next;
			}
			p->next = 0;
			ps->next = head;
			break;
		}
		case down:
		{
		casedown:
			for (; ps->next != 0;)
			{
				ps = ps->next;
			}
			gotoxy(ps->x, ps->y);
			std::cout << ' ';
			ps->x = head->x;
			ps->y = head->y + 1;
			Head = ps;
			Head->direct = down;
			for (; p->next->next != 0;)
			{
				p = p->next;
			}
			p->next = 0;
			ps->next = head;
			break;
		}
		case left:
		{
		caseleft:
			for (; ps->next != 0;)
			{
				ps = ps->next;
			}
			gotoxy(ps->x, ps->y);
			std::cout << ' ';
			ps->x = head->x - 1;
			ps->y = head->y;
			Head = ps;
			Head->direct = left;
			for (; p->next->next != 0;)
			{
				p = p->next;
			}
			p->next = 0;
			ps->next = head;
			break;
		}
		case right:
		{
		caseright:
			for (; ps->next != 0;)
			{
				ps = ps->next;
			}
			gotoxy(ps->x, ps->y);
			std::cout << ' ';
			ps->x = head->x + 1;
			ps->y = head->y;
			Head = ps;
			Head->direct = right;
			for (; p->next->next != 0;)
			{
				p = p->next;
			}
			p->next = 0;
			ps->next = head;
			break;
		}
		}
	}
}

void ChangeDirect(Snake *head)
{
	Snake *p = head;
	for (; p->next != 0;)
	{
		if (p->y == p->next->y)
		{
			if (p->x > p->next->x)
			{
				p->next->direct = right;
			}
			else
			{
				p->next->direct = left;
			}
		}
		else
		{
			if (p->y > p->next->y)
			{
				p->next->direct = down;
			}
			else
			{
				p->next->direct = up;
			}
		}
		p = p->next;
	}
}

void ProductFood(Snake *head)
{
Timeagain:
	Snake *p = head;
	srand(unsigned(time(0)));
	a = rand() % (RightBorder - 3) + 1;
	b = rand() % (DownBorder - 3) + 1;

	for (; p != 0;)
	{
		if (p->x == a && p->y == b)
		{
			goto Timeagain;
		}
		else
		{
			p = p->next;
		}
	}
	gotoxy(a, b);
	std::cout << '*';
}

bool JudgeAboutFood(Snake *head, int a, int b, int gameclass)
{
	if (head->x == a && head->y == b)
	{
		grade = grade + (50 - gameclass / 10);
		for (int i = 0; i <= D - 1; i++)
		{
			CreateNext(Head);
		}
		return true;
	}
	return false;
}

void PrintMap()
{
	gotoxy(0, 0);
	std::cout << '+';
	gotoxy(RightBorder - 1, 0);
	std::cout << '+';
	gotoxy(0, DownBorder - 1);
	std::cout << '+';
	gotoxy(RightBorder - 1, DownBorder - 1);
	std::cout << '+';
	for (int i = 1; i <= RightBorder - 2; i++)
	{
		gotoxy(i, 0);
		std::cout << '-';
		gotoxy(i, DownBorder - 1);
		std::cout << '-';
	}
	for (int i = 1; i <= DownBorder - 2; i++)
	{
		gotoxy(0, i);
		std::cout << '|';
		gotoxy(RightBorder - 1, i);
		std::cout << '|';
	}
}

void Time_Wait(int t)
{
	Sleep(t);
}

void CGameInterface(int g, std::string name)
{
	PrintMap();
	gotoxy(RightBorder / 3 * 2, DownBorder + 2);
	std::cout << "最高分：" << g;
	gotoxy(RightBorder / 3 * 2, DownBorder + 3);
	std::cout << "保持者：" << name;
	gotoxy(RightBorder / 6, DownBorder + 2);
	std::cout << "你的分数：" << grade;
}

void EGameInterface(int g, std::string name)
{
	PrintMap();
	gotoxy(RightBorder / 3 * 2, DownBorder + 2);
	std::cout << "The highest grade is:" << g;
	gotoxy(RightBorder / 3 * 2, DownBorder + 3);
	std::cout << "The creater is:" << name;
	gotoxy(RightBorder / 6, DownBorder + 2);
	std::cout << "Your grade:" << grade;
}

void CUserInterface(int choice)
{
	switch (choice)
	{
	case MainMenu:
	{
		gotoxy(RightBorder / 2 - 5, DownBorder / 5);
		std::cout << "贪 吃 蛇";
		gotoxy(RightBorder / 2 - 7, DownBorder / 5 + 2);
		std::cout << "1.开始游戏";
		gotoxy(RightBorder / 2 - 7, DownBorder / 5 + 4);
		std::cout << "2.最高分";
		gotoxy(RightBorder / 2 - 7, DownBorder / 5 + 6);
		std::cout << "3.选项";
		gotoxy(RightBorder / 2 - 7, DownBorder / 5 + 8);
		std::cout << "4.退出";
		break;
	}
	case DifficultyChoice:
	{
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 - 2);
		std::cout << "速度选择";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3);
		std::cout << "1.慢速";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 2);
		std::cout << "2.中速";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 4);
		std::cout << "3.快速";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 6);
		std::cout << "4.超速";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 8);
		std::cout << "5.极速";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 10);
		std::cout << "6.光速";
		break;
	}
	case NextChoice:
	{
		gotoxy(RightBorder / 3 - 21, DownBorder / 3);
		std::cout << "请输入当你吃到食物时所想要增加的节数（只允许1到5节）：";
		break;
	}
	case GradeName:
	{
		gotoxy(RightBorder / 3 - 21, DownBorder / 3);
		std::cout << "请输入你的名字：";
		break;
	}
	case MaxGrade:
	{
		gotoxy(RightBorder / 3 - 21, DownBorder / 3);
		std::cout << "最高分：";
		gotoxy(RightBorder / 3 - 21, DownBorder / 3 + 2);
		std::cout << "保持者：";
		break;
	}
	case Settings:
	{
		gotoxy(RightBorder / 3 - 7, DownBorder / 3);
		std::cout << "1.颜色";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 2);
		std::cout << "2.语言";
		break;
	}
	case ColorChoice:
	{
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 - 2);
		std::cout << "颜色选择";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3);
		std::cout << "1.亮白底黑字";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 2);
		std::cout << "2.淡紫色底黑字";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 4);
		std::cout << "3.灰底黑字";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 6);
		std::cout << "4.淡黄色底淡红色字";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 8);
		std::cout << "5.淡浅绿色底淡蓝色字";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 10);
		std::cout << "6.默认";
		break;
	}
	case LanguageChoice:
	{
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 - 2);
		std::cout << "语言选择";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3);
		std::cout << "1.中文";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 2);
		std::cout << "2.英文";
		break;
	}
	}
}

void EUserInterface(int choice)
{
	switch (choice)
	{
	case MainMenu:
	{
		gotoxy(RightBorder / 2 - 5, DownBorder / 5);
		std::cout << "Snake";
		gotoxy(RightBorder / 2 - 7, DownBorder / 5 + 2);
		std::cout << "1.Start";
		gotoxy(RightBorder / 2 - 7, DownBorder / 5 + 4);
		std::cout << "2.The highest grade";
		gotoxy(RightBorder / 2 - 7, DownBorder / 5 + 6);
		std::cout << "3.Settings";
		gotoxy(RightBorder / 2 - 7, DownBorder / 5 + 8);
		std::cout << "4.Exit";
		break;
	}
	case DifficultyChoice:
	{
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 - 2);
		std::cout << "Speed choice";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3);
		std::cout << "1.Low speed";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 2);
		std::cout << "2.Medium choice";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 4);
		std::cout << "3.Fast speed";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 6);
		std::cout << "4.Over speed";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 8);
		std::cout << "5.Limit speed";
		gotoxy(RightBorder / 2 - 7, DownBorder / 3 + 10);
		std::cout << "6.Light speed";
		break;
	}
	case NextChoice:
	{
		gotoxy(RightBorder / 3 - 21, DownBorder / 3);
		std::cout << "Please print the length you want to get when you eat a food(just for 1 to 5):";
		break;
	}
	case GradeName:
	{
		gotoxy(RightBorder / 3 - 21, DownBorder / 3);
		std::cout << "Please print your name:";
		break;
	}
	case MaxGrade:
	{
		gotoxy(RightBorder / 3 - 21, DownBorder / 3);
		std::cout << "The highest grade:";
		gotoxy(RightBorder / 3 - 21, DownBorder / 3 + 2);
		std::cout << "The creater is:";
		break;
	}
	case Settings:
	{
		gotoxy(RightBorder / 3 - 7, DownBorder / 3);
		std::cout << "1.Color";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 2);
		std::cout << "2.Language";
		break;
	}
	case ColorChoice:
	{
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 - 2);
		std::cout << "Color choice";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3);
		std::cout << "1.The light background and the black word";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 2);
		std::cout << "2.The lavender background and the black word";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 4);
		std::cout << "3.The gray background and the black word";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 6);
		std::cout << "4.The canary background and the reddish word";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 8);
		std::cout << "5.The light green background and the nattier blue word";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 10);
		std::cout << "6.Default";
		break;
	}
	case LanguageChoice:
	{
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 - 2);
		std::cout << "Language choice";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3);
		std::cout << "1.Chinese";
		gotoxy(RightBorder / 3 - 7, DownBorder / 3 + 2);
		std::cout << "2.English";
		break;
	}
	}
}