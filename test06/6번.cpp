#include <iostream>
#include <random>
#define SIZE 30
using namespace std;

enum Direction
{
	Up = 0,
	Right = 2,
	Down = 1,
	Left = 3
};

class Point
{
private:
	int x;
	int y;
public:
	Point() :x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}
	~Point() {}

	void setX(int a) { x = a; }
	void setY(int a) { y = a; }
	void Set(int a, int b) { x = a; y = b; }
	int GetX() { return x; }
	int GetY() { return y; }
	bool MovePoint(int dir)
	{
		switch (dir)
		{
		case Up:
			if (y > 0)
			{
				y--;
				return true;
			}
			else return false;
			break;
		case Right:
			if (x < SIZE - 1)
			{
				x++;
				return true;
			}
			else return false;
			break;
		case Down:
			if (y < SIZE - 1)
			{
				y++;
				return true;
			}
			else return false;
			break;
		case Left:
			if (x > 0)
			{
				x--;
				return true;
			}
			else return false;
			break;
		}
	}
};

bool MoveBuf(int arr[SIZE][SIZE], int dir, Point &p, int n, int& a)
{
	for (int i = 0; i < n - 1; i++)
	{
		if (p.MovePoint(dir) == false)
		{
			if (i == 0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}
		a++;
		arr[p.GetY()][p.GetX()] = a;
	}
	return true;
}
void PrintArr(int arr[SIZE][SIZE]);

bool MakeRoute(int arr[SIZE][SIZE], Point& p, int& a)
{
	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<int> R_Range(2, 8);
	uniform_int_distribution<int> R_Direction(0, 9);

	bool GoRight = false;
	bool success = false;
	int r = 0;
	int up = 0, down = 0, left = 0, right = 0;

	while (p.GetX() != 29 || p.GetY() != 29)
	{
		if (p.GetX() == 29 && p.GetY() > 21 && GoRight == false)
		{
			MoveBuf(arr, Down, p, SIZE - p.GetY(), a);
			GoRight = true;
			down++;
		}
		else if (p.GetY() == 29 && p.GetX() > 21 && GoRight == true)
		{
			MoveBuf(arr, Right, p, SIZE - p.GetX(), a);
			GoRight = false;
			right++;
		}
		else if (p.GetX() == 29 && GoRight == true)
		{
			MoveBuf(arr, Left, p, R_Range(gen), a);
			GoRight = false;
			left++;
		}
		else if (p.GetY() == 29 && GoRight == false)
		{
			MoveBuf(arr, Up, p, R_Range(gen), a);
			GoRight = true;
			up++;
		}
		else
		{
			if (p.GetX() == 0 && p.GetY() == 0)
			{
				r = R_Range(gen) % 2;
				if (r == 0)
				{
					GoRight = true;
				}
				else
				{
					GoRight = false;
				}
			}
			if (GoRight == false)
			{
				r = R_Direction(gen);

				if (r > 2)
				{
					down++;
					success = MoveBuf(arr, Down, p, R_Range(gen), a);
				}
				else
				{
					up++;
					success = MoveBuf(arr, Up, p, R_Range(gen), a);
				}
				if (success == true)GoRight = true;
				else GoRight = false;
			}
			else
			{
				r = R_Direction(gen);

				if (r > 2)
				{
					right++;
					success = MoveBuf(arr, Right, p, R_Range(gen), a);
				}
				else
				{
					left++;
					success = MoveBuf(arr, Left, p, R_Range(gen), a);
				}
				if (success == true)GoRight = false;
				else GoRight = true;
			}

		}

	}
	if (up == 0 || down == 0 || left == 0 || right == 0)
		return false;
	else
		return true;
}

void PrintArr(int arr[SIZE][SIZE])
{
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < SIZE; j++)
		{
			cout.width(4);
			cout.fill(' ');
			cout << arr[i][j];
		}
		cout << endl;
	}
}

int main()
{
	int arr[SIZE][SIZE] = { '0' };
	Point buf;
	int num = 1;
	arr[0][0] = 1;
	
	char c;

	int n = 0;

	while (MakeRoute(arr, buf, num) == false)
	{
		for (int i = 0; i < SIZE; i++)
		{
			for (int j = 0; j < SIZE; j++)
			{
				arr[i][j] = 0;
			}
		}
		num = 1;
		arr[0][0] = 1;
		buf.Set(0, 0);
	}
	PrintArr(arr);
	while (1)
	{
		cout << "r: 경로를 다시만든다" << endl
			<< "x/z : 한칸 우측/ 좌측으로 옮김." << endl
			<< "q: 프로그램 종료" << endl
			<< "order: ";
		cin >> c;

		switch (c)
		{
		case 'r':
			for (int i = 0; i < SIZE; i++)
			{
				for (int j = 0; j < SIZE; j++)
				{
					arr[i][j] = 0;
				}
			}
			num = 1;
			arr[0][0] = 1;
			buf.Set(0, 0);
			
			while (MakeRoute(arr, buf, num) == false) {}
			PrintArr(arr);
			break;
		case'x':
			for (int i = 0; i < SIZE; i++)
			{
				n = arr[i][SIZE - 1];
				for (int j = SIZE - 1; j > 0; j--)
				{
					arr[i][j] = arr[i][j - 1];

				}
				arr[i][0] = n;
			}

			PrintArr(arr);

			break;
		case 'z':
			for (int i = 0; i < SIZE; i++)
			{
				n = arr[i][0];
				for (int j = 0; j <SIZE - 1; j++)
				{
					arr[i][j] = arr[i][j + 1];

				}
				arr[i][SIZE - 1] = n;
			}
			PrintArr(arr);

			break;
		case'q':
			return 0;
			break;
		}
	}
	
}