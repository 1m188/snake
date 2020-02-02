#include "snake.h"
#include "iostream"
#include "windows.h"
#include "tchar.h"
#include "fstream"
#include "string"
#include "conio.h"
#include "cctype"

int main()
{
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Cursor_info);
GameStart:
	snakehead = Snake();
	Head = &snakehead;
	gameclass = 0;
	grade = 0;
	int languagechoice = 1;
	int colorchoice = 6;
	int FileGrade;
	int maxgrade = 0;
	std::string maxgradename;
	std::string FileName;
	std::fstream Grade;
	std::ofstream CreateGrade;
	std::fstream Set;
	std::ofstream CreateSet;
	std::string Name;

	Grade.open("grade.txt");
	if (Grade)
	{
		Grade >> FileGrade;
		Grade >> FileName;
		Grade.close();
	}
	else
	{
		Grade.close();
		CreateGrade.open("grade.txt");
		CreateGrade.close();
		Grade.open("grade.txt");
		FileGrade = 0;
		FileName = "";
		Grade.close();
	}
	if (FileGrade < 0)
	{
		FileGrade = 0;
	}
	Set.open("set.set");
	if (Set)
	{
		Set >> colorchoice;
		Set >> languagechoice;
		Set.close();
	}
	else
	{
		Set.close();
		CreateSet.open("set.set");
		CreateSet.close();
		Set.open("set.set");
		Set << colorchoice;
		Set << '\n';
		Set << languagechoice;
		Set.close();
	}

MainMenu:
	switch (colorchoice)
	{
	case 1:
	{
		system("color F0");
		break;
	}
	case 2:
	{
		system("color D0");
		break;
	}
	case 3:
	{
		system("color 70");
		break;
	}
	case 4:
	{
		system("color EC");
		break;
	}
	case 5:
	{
		system("color B9");
		break;
	}
	case 6:
	{
		system("color 07");
		break;
	}
	}
	system("mode con cols=120 lines=35");
	if (languagechoice == 1)
	{
		CUserInterface(MainMenu);
	}
	else
	{
		EUserInterface(MainMenu);
	}
	for (;;)
	{
		switch (JudgeCin())
		{
		case '1':
		{
			system("cls");
			if (languagechoice == 1)
			{
				CUserInterface(DifficultyChoice);
			}
			else
			{
				EUserInterface(DifficultyChoice);
			}
			for (;;)
			{
				switch (JudgeCin())
				{
				case '1':
				{
					gameclass = 100;
					break;
				}
				case '2':
				{
					gameclass = 75;
					break;
				}
				case '3':
				{
					gameclass = 45;
					break;
				}
				case '4':
				{
					gameclass = 30;
					break;
				}
				case '5':
				{
					gameclass = 5;
					break;
				}
				case '6':
				{
					system("cls");
					if (languagechoice == 1)
					{
						CUserInterface(NextChoice);
					}
					else
					{
						EUserInterface(NextChoice);
					}
					for (;;)
					{
						if (std::cin >> D && D > 0 && D < 6)
						{
							break;
						}
						else
						{
							system("cls");
							if (languagechoice == 1)
							{
								CUserInterface(NextChoice);
							}
							else
							{
								EUserInterface(NextChoice);
							}
							std::cin.clear();
							std::cin.get();
						}
					}
					system("cls");
					PrintMap();
					CreateNext(Head);
					CreateNext(Head);
					ShowAllBody(Head);
					ProductFood(Head);
					if (languagechoice == 1)
					{
						CGameInterface(FileGrade, FileName);
					}
					else
					{
						EGameInterface(FileGrade, FileName);
					}
					if (languagechoice == 1)
					{
						MessageBox(hout, _T("根据光速不变原理与洛伦兹变换，在达到光速后发生了相对论效应，贪吃蛇参考系的时间相对于玩家参考系无限延长，即玩家参考系中的游戏永不结束，但是游戏参考系中的游戏已经结束了。"), _T("光速"), MB_OK | MB_ICONWARNING);
					}
					else
					{
						MessageBox(hout, _T("According to the theory of the light's speed not change and the vary of the Lorentz,there are a relativistic effect after reach the light speed,the time of the snake is no limit to the player ,it is said that the game in the player's reference will not be end forever,but the game in the snake's reference has been end."), _T("The light speed"), MB_OK | MB_ICONWARNING);
					}
					goto END;
				}
				}
				system("cls");
				if (languagechoice == 1)
				{
					CUserInterface(NextChoice);
				}
				else
				{
					EUserInterface(NextChoice);
				}
				for (;;)
				{
					SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
					if (std::cin >> D && D > 0 && D < 6)
					{
						std::cin.get();
						SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Cursor_info);
						break;
					}
					else
					{
						system("cls");
						if (languagechoice == 1)
						{
							CUserInterface(NextChoice);
						}
						else
						{
							EUserInterface(NextChoice);
						}
						std::cin.clear();
						std::cin.get();
					}
				}
				system("cls");
				PrintMap();
				CreateNext(Head);
				CreateNext(Head);
				ProductFood(Head);
				ShowAllBody(Head);
				if (languagechoice == 1)
				{
					CGameInterface(FileGrade, FileName);
				}
				else
				{
					EGameInterface(FileGrade, FileName);
				}
				for (;;)
				{
					Move(Head);
					ChangeDirect(Head);
					ShowBody(Head);
					if (JudgeAboutFood(Head, a, b, gameclass) == true)
					{
						ShowAllBody(Head);
						if (languagechoice == 1)
						{
							CGameInterface(FileGrade, FileName);
						}
						else
						{
							EGameInterface(FileGrade, FileName);
						}
						ProductFood(Head);
					}
					if (Judgeiflive(Head) == false)
					{
						goto END;
					}
					Time_Wait(gameclass);
				}
			}
			break;
		}
		case '2':
		{
			Grade.open("grade.txt");
			Grade >> maxgrade;
			Grade >> maxgradename;
			Grade.close();
			system("cls");
			if (languagechoice == 1)
			{
				CUserInterface(MaxGrade);
				gotoxy(RightBorder / 3 - 13, DownBorder / 3);
				std::cout << maxgrade;
				gotoxy(RightBorder / 3 - 13, DownBorder / 3 + 2);
				std::cout << maxgradename;
			}
			else
			{
				EUserInterface(MaxGrade);
				gotoxy(RightBorder / 3 - 3, DownBorder / 3);
				std::cout << maxgrade;
				gotoxy(RightBorder / 3 - 6, DownBorder / 3 + 2);
				std::cout << maxgradename;
			}
			while (JudgeCin())
			{
				goto MainMenu;
			}
			break;
		}
		case '3':
		{
			system("cls");
			if (languagechoice == 1)
			{
				CUserInterface(Settings);
			}
			else
			{
				EUserInterface(Settings);
			}
			switch (JudgeCin())
			{
			case '1':
			{
				system("cls");
				if (languagechoice == 1)
				{
					CUserInterface(ColorChoice);
				}
				else
				{
					EUserInterface(ColorChoice);
				}
				for (;;)
				{
					switch (JudgeCin())
					{
					case '1':
					{
						colorchoice = 1;
						Set.open("set.set");
						Set << colorchoice;
						Set << '\n';
						Set << languagechoice;
						Set.close();
						system("color F0");
						goto MainMenu;
					}
					case '2':
					{
						colorchoice = 2;
						Set.open("set.set");
						Set << colorchoice;
						Set << '\n';
						Set << languagechoice;
						Set.close();
						system("color D0");
						goto MainMenu;
					}
					case '3':
					{
						colorchoice = 3;
						Set.open("set.set");
						Set << colorchoice;
						Set << '\n';
						Set << languagechoice;
						Set.close();
						system("color 70");
						goto MainMenu;
					}
					case '4':
					{
						colorchoice = 4;
						Set.open("set.set");
						Set << colorchoice;
						Set << '\n';
						Set << languagechoice;
						Set.close();
						system("color EC");
						goto MainMenu;
					}
					case '5':
					{
						colorchoice = 5;
						Set.open("set.set");
						Set << colorchoice;
						Set << '\n';
						Set << languagechoice;
						Set.close();
						system("color B9");
						goto MainMenu;
					}
					case '6':
					{
						colorchoice = 6;
						Set.open("set.set");
						Set << colorchoice;
						Set << '\n';
						Set << languagechoice;
						Set.close();
						system("color 07");
						goto MainMenu;
					}
					}
				}
				break;
			}
			case '2':
			{
				system("cls");
				if (languagechoice == 1)
				{
					CUserInterface(LanguageChoice);
				}
				else
				{
					EUserInterface(LanguageChoice);
				}
				for (;;)
				{
					switch (JudgeCin())
					{
					case '1':
					{
						languagechoice = 1;
						Set.open("set.set");
						Set << colorchoice;
						Set << '\n';
						Set << languagechoice;
						Set.close();
						goto MainMenu;
					}
					case '2':
					{
						languagechoice = 2;
						Set.open("set.set");
						Set << colorchoice;
						Set << '\n';
						Set << languagechoice;
						Set.close();
						goto MainMenu;
					}
					}
				}
				break;
			}
			default:
			{
				goto MainMenu;
			}
			}
			break;
		}
		case '4':
		{
			return 0;
			break;
		}
		}
	}

END:
	if (grade > FileGrade)
	{
		CreateGrade.open("grade.txt", std::ios::trunc);
		CreateGrade.close();
		Grade.open("grade.txt");
		if (languagechoice == 1)
		{
			MessageBox(hout, _T("恭喜你的分数超过了最高分，这是个新纪录！"), _T("新纪录"), MB_OK | MB_ICONWARNING);
		}
		else
		{
			MessageBox(hout, _T("I'm very glad to tell you that your grade has overed the highest grade,this is a new record!"), _T("New record"), MB_OK | MB_ICONWARNING);
		}
		system("cls");
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
		if (languagechoice == 1)
		{
			CUserInterface(GradeName);
		}
		else
		{
			EUserInterface(GradeName);
		}
		getline(std::cin, Name);
		Grade << grade;
		Grade << '\n';
		Grade << Name;
		Grade.close();
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &Cursor_info);
		if (languagechoice == 1)
		{
			MessageBox(hout, _T("新纪录诞生了！"), _T("记录成功"), MB_OK | MB_ICONWARNING);
		}
		else
		{
			MessageBox(hout, _T("New record has come up!"), _T("Record success"), MB_OK | MB_ICONWARNING);
		}
		goto GameStart;
	}
	if (languagechoice == 1)
	{
		MessageBox(hout, _T("可惜你没有创造一个新纪录，是否要再来一局？"), _T("失败"), MB_YESNO | MB_ICONWARNING);
	}
	else
	{
		MessageBox(hout, _T("It's very pity that you can't create a new record,do you want to play again?"), _T("Fail"), MB_YESNO | MB_ICONWARNING);
	}
	goto GameStart;
}