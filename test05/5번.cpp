#include <iostream>
#include <cmath>

#define MaxPoint 10

using namespace std;

class vect
{
private:
	int x;
	int y;
	int z;
	double dis_ori;
	int index;
	bool IsFull = false;
public:
	vect() { x = 0; y = 0; z = 0; dis_ori = 0; index = -1; }
	~vect() {}
	void Cal_Dis() {
		dis_ori = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
	}
	void setX(int x){vect::x = x; IsFull = true; Cal_Dis();}
	void setY(int y) { vect::y = y; IsFull = true; Cal_Dis();}
	void setZ(int z) { vect::z = z; IsFull = true; Cal_Dis(); }
	void set(int a, int b, int c) { x = a; y = b; z = c; IsFull = true; Cal_Dis(); }
	void del() { x = 0; y = 0; z = 0; IsFull = false; dis_ori = 0; index = -1; }
	bool getIsFull() { return IsFull; }
	double getDisOri() { return dis_ori; }
	void PrintVect() { cout << "(" << x << ", " << y << ", " << z << ")" << endl; }
	void setind(int n) { index = n; }
	int getind() { return index; }
};

void Change(vect &a, vect &b)
{
	vect c;
	c = a;
	a = b;
	b = c;
}

int main()
{
	vect v[MaxPoint];
	char ord;
	int x, y, z;
	int V_num = 0; //벡터의 수
	int Max = 0;
	int MaxN = 0;
	int Min = 0;
	int MinN = 0;
	int v_buf = 0;
	bool thereis_m = true;
	bool sb = false;
	while (1)
	{
		cout << "+ x y z: 리스트의 맨 위에 입력" << endl;
		cout << "-: 리스트의 맨 위에서 삭제" << endl;
		cout << "e x y z: 리스트의 맨 아래에 입력" << endl;
		cout << "d: 리스트의 맨 아래에서 삭제" << endl;
		cout << "l: 리스트에 저장된 점의 개수 출력" << endl;
		cout << "c: 리스트를 비운다" << endl;
		cout << "m: 원점에서 가장 먼 거리에 있는 점의 좌표 출력" << endl;
		cout << "n: 원점에서 가장 가까운 거리에 있는 점의 좌표 출력" << endl;
		cout << "s: 원점과의 거리를 정렬하여 오름차순 정렬" << endl;
		cout << "q: 프로그램 종료" << endl;
		cout << "명령: ";
		cin >> ord;
		char c = getchar();
		switch (ord)
		{
		case '+':
			cin >> x >> y >> z;
			c = getchar();
			
			if (v_buf != MaxPoint)
			{
				v[v_buf].set(x, y, z);
				v_buf++;
			}
			else if (v_buf == MaxPoint)
			{
				for (int i = MaxPoint - 1; i >= 0; i--)
				{
					if (v[i].getIsFull() == false) {
						v[i].set(x, y, z);
						break;
					}
					else if (v[i].getIsFull() == true && i == 0)
						cout << "리스트가 모두 차있습니다." << endl;
				}
			}

			for (int i = MaxPoint - 1; i >= 0; i--)
			{
				cout << i << ": ";
				if (v[i].getIsFull() == true)
					v[i].PrintVect();
				else
					cout << endl;
			}
			break;
		case '-':
			for (int i = MaxPoint - 1; i >= 0; i--)
			{
				if (v[i].getIsFull() == true)
				{
					v[i].del();
					v_buf--;
					break;
				}
				else if (v[i].getIsFull() == false && i == 0)
					cout << "리스트가 없습니다." << endl;
			}
			for (int i = MaxPoint - 1; i >= 0; i--)
			{
				cout << i << ": ";
				if (v[i].getIsFull() == true)
					v[i].PrintVect();
				else
					cout << endl;
			}
			break;
		case 'e':
				cin >> x >> y >> z;
				c = getchar();

				if (v_buf != MaxPoint)
				{
					for (int i = MaxPoint - 1; i > 0; i--)
					{
						Change(v[i], v[i - 1]);
					}

					v[0].set(x, y, z);

					v_buf++;
				}
				else if (v_buf == MaxPoint)
				{
					for (int i = 0; i < MaxPoint; i++)
					{
						if (v[i].getIsFull() == false) {
							v[i].set(x, y, z);
							break;
						}
						else if (v[i].getIsFull() == true && i == MaxPoint - 1);
							cout << "리스트가 모두 차있습니다." << endl;
					}
				}
				for (int i = MaxPoint - 1; i >= 0; i--)
				{
					cout << i << ": ";
					if (v[i].getIsFull() == true)
						v[i].PrintVect();
					else
						cout << endl;
				}
				break;
		case 'd':
			for (int i = 0; i < MaxPoint; i++)
			{
				if (v[i].getIsFull() == true) {
					v[i].del();
					break;
				}
				else if (v[i].getIsFull() == false && i == MaxPoint - 1);
			}
			for (int i = MaxPoint - 1; i >= 0; i--)
			{
				cout << i << ": ";
				if (v[i].getIsFull() == true)
					v[i].PrintVect();
				else
					cout << endl;
			}
			break;
		case 'l':
			V_num = 0;
			for (int i = 0; i < MaxPoint; i++)
			{
				if (v[i].getIsFull() == true)
					V_num++;
			}
			cout << "점의 개수는 " << V_num << endl;
			break;
		case 'c':
			for (int i = 0; i < MaxPoint; i++)
			{
				v[i].del();
			}
			for (int i = MaxPoint - 1; i >= 0; i--)
			{
				cout << i << ": ";
				if (v[i].getIsFull() == true)
					v[i].PrintVect();
				else
					cout << endl;
			}
			v_buf = 0;
			break;
		case'm':
			Max = -1;
			MaxN = -1;
			thereis_m = true;
			for (int i = 0; i < MaxPoint; i++)
			{
				if (v[i].getIsFull() == true) {
					if (Max == -1 || Max < v[i].getDisOri())
					{
						Max = v[i].getDisOri();
						MaxN = i;
					}
				}
				if (Max == -1 && i == MaxPoint - 1)
				{
					thereis_m = false;
				}
			}

			if (thereis_m == true)
			{
				cout << "원점에서 먼 좌표값: ";
				v[MaxN].PrintVect();
			}
			break;
		case 'n':
			Min = -1;
			MinN = -1;
			thereis_m = true;
			for (int i = 0; i < MaxPoint; i++)
			{
				if (v[i].getIsFull() == true) {
					if (Min == -1 || Min > v[i].getDisOri())
					{
						Min = v[i].getDisOri();
						MinN = i;
					}
				}
				if (Min == -1 && i == MaxPoint - 1)
				{
					thereis_m = false;
				}
			}

			if (thereis_m == true)
			{
				cout << "원점에서 먼 좌표값: ";
				v[MinN].PrintVect();
			}
			break;
		case 's':
			if (sb == false)
			{
				for (int i = 0; i < MaxPoint; i++)
				{
					if (v[i].getIsFull() == true) {
						v[i].setind(i);
					}
				}

				for (int i = 0; i < MaxPoint - 1; i++)
				{
					Min = -1;
					MinN = -1;
					thereis_m = true;
					for (int j = i; j < MaxPoint; j++)
					{
						if (v[j].getIsFull() == true) {
							if (Min == -1 || Min > v[j].getDisOri())
							{
								Min = v[j].getDisOri();
								MinN = j;
							}
						}
						if (Min == -1 && j == MaxPoint - 1)
						{
							thereis_m = false;
						}
					}
					if (thereis_m == false)
						break;
					else
					{
						Change(v[i], v[MinN]);
					}
				}
				sb = true;
			}
			else
			{
				for (int i = 0; i < MaxPoint; i++)
				{
					for (int j = 0; j < MaxPoint; j++)
					{
						if (v[j].getind() == i) {
							Change(v[i], v[j]);
							break;
						}
					}
				}
				sb = false;
			}

			for (int i = MaxPoint - 1; i >= 0; i--)
			{
				cout << i << ": ";
				if (v[i].getIsFull() == true)
					v[i].PrintVect();
				else
					cout << endl;
			}
			break;
		case 'q':
			return 0;
			break;
		}
	}
	return 0;
}