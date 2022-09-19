#include <iostream>
#include <random>
#include <windows.h >

#define SIZE 4
using namespace std;

void ColorPrint(char c)
{
	unsigned short text = 15;

	switch (c)
	{
	case 'A':
		text = 1;
		break;
	case 'B':
		text = 2;
		break;
	case 'C':
		text = 3;
		break;
	case 'D':
		text = 4;
		break;
	case 'E':
		text = 5;
		break;
	case 'F':
		text = 6;
		break;
	case 'G':
		text = 7;
		break;
	case 'H':
		text = 14;
		break;
	case '0':
		text = 8;
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
	cout << c << " ";
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
}

void PrintBoard(char arr[SIZE][SIZE], int score, int count)
{
	cout << "=======================" << endl;
	cout << "score : " << score << " // count : " << count << endl;
	cout << "-----------------------" << endl;
	cout << "  a b c d" << endl;
	for (int i = 0; i < SIZE; i++)
	{
		cout << i + 1 <<" ";
		for (int j = 0; j < SIZE; j++)
		{
			ColorPrint(arr[i][j]);
		}
		cout << endl;
	}
	cout << "=======================" << endl;

}

void ResetBoard(char Board[SIZE][SIZE], char arr[SIZE][SIZE])
{
	string s = "AABBCCDDEEFFGGHH";
	random_device rd;
	mt19937 gen(rd());
	int rand = 0;

	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			Board[i][j] = '*';
		}
	}
 
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			uniform_int_distribution<int> ran(0, s.length() - 1);
			rand = ran(gen);
			arr[i][j] = s[rand];

			s = s.substr(0, (rand)) + s.substr((rand + 1), (s.length() - 1));
		}
	}
}

int coordinate(char c)
{
	switch (c)
	{
	case 'a':
		return 0;
		break;
	case 'b':
		return 1;
		break;
	case 'c':
		return 2;
		break;
	case 'd':
		return 3;
		break;
	}
	return -1;
}
int coordinate(int c)
{
	return c - 1;
}

bool InputCard(char Board[SIZE][SIZE], char &c, int &i)
{
	while(1)
	{
		cin >> c;
		if (c == 'r')
		{
			cout << "게임을 리셋합니다." << endl;
			return false;
		}
		else if (c < 'a' || c >'d')
		{
			cout << "올바른 값을 입력해주세요." << endl;
			continue;
		}
		cin >> i;
		if (i < 1 || i > 4)
		{
			cout << "올바른 값을 입력해주세요." << endl;
			continue;
		}

		if (Board[coordinate(i)][coordinate(c)] == '*')
		{
			break;
		}
		else
		{
			cout << "비어있는 칸이 아닙니다." << endl;
		}
	}
	return true;
}
bool CheckBoard(char Board[SIZE][SIZE])
{
	int IsEmpty = true;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			if (Board[i][j] == '*')
				IsEmpty = false;
		}
	}
	return IsEmpty;
}
int main()
{
	char Board[SIZE][SIZE];
	char arr[SIZE][SIZE];
	char c1 = 0, c2 = 0;
	int i1 = 0, i2 = 0;
	int score = 0, count = 20;
	char e = 0;
	ResetBoard(Board, arr);
	PrintBoard(arr, score, count);

	while (count > 0)
	{
		PrintBoard(Board, score, count);

		cout << "input card 1: ";
		if (InputCard(Board, c1, i1) == false)
		{
			PrintBoard(arr, score, count);

			score = 0; count = 20;
			ResetBoard(Board, arr);

			continue;
		}

		Board[coordinate(i1)][coordinate(c1)] = '0';
		PrintBoard(Board, score, count);

		cout << "input card 2: ";
		if (InputCard(Board, c2, i2) == false)
		{
			PrintBoard(arr, score, count);

			score = 0; count = 20;

			ResetBoard(Board, arr);

			continue;
		}

		Board[coordinate(i2)][coordinate(c2)] = '0';
		PrintBoard(Board, score, count);

		cout << "click enter: " << endl;
		while (e != '\n')
		{
			char c = getchar();
			e = getchar();
		}
		e = 0;

		Board[coordinate(i1)][coordinate(c1)] = arr[coordinate(i1)][coordinate(c1)];
		Board[coordinate(i2)][coordinate(c2)] = arr[coordinate(i2)][coordinate(c2)];
		if (Board[coordinate(i1)][coordinate(c1)] != Board[coordinate(i2)][coordinate(c2)])
		{
			PrintBoard(Board, score, count);

			cout << "다른 카드입니다." << endl;
			Board[coordinate(i1)][coordinate(c1)] = '*';
			Board[coordinate(i2)][coordinate(c2)] = '*';
		}
		else
		{
			score = score + (count * 10);
		}

		if (CheckBoard(Board))
		{
			PrintBoard(Board, score, count);

			count--;

			cout << "*****************************" << endl;
			cout << "***********!clear!***********" << endl;
			cout << "*****************************" << endl;
			cout << "*count: " << count << "********************" << endl;
			cout << "*****************************" << endl;
			cout << "*score: " << score << "******************" << endl;
			cout << "*****************************" << endl;
			cout << "*****************************" << endl;
			break;
		}
		count--;
	}

	if (count == 0)
	{
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "*****************************" << endl;
		cout << "         ! you lose !        " << endl;
		cout << "*****************************" << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
	}
}