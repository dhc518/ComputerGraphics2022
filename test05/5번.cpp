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
	int V_num = 0; //������ ��
	int Max = 0;
	int MaxN = 0;
	int Min = 0;
	int MinN = 0;
	int v_buf = 0;
	bool thereis_m = true;
	bool sb = false;
	while (1)
	{
		cout << "+ x y z: ����Ʈ�� �� ���� �Է�" << endl;
		cout << "-: ����Ʈ�� �� ������ ����" << endl;
		cout << "e x y z: ����Ʈ�� �� �Ʒ��� �Է�" << endl;
		cout << "d: ����Ʈ�� �� �Ʒ����� ����" << endl;
		cout << "l: ����Ʈ�� ����� ���� ���� ���" << endl;
		cout << "c: ����Ʈ�� ����" << endl;
		cout << "m: �������� ���� �� �Ÿ��� �ִ� ���� ��ǥ ���" << endl;
		cout << "n: �������� ���� ����� �Ÿ��� �ִ� ���� ��ǥ ���" << endl;
		cout << "s: �������� �Ÿ��� �����Ͽ� �������� ����" << endl;
		cout << "q: ���α׷� ����" << endl;
		cout << "���: ";
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
						cout << "����Ʈ�� ��� ���ֽ��ϴ�." << endl;
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
					cout << "����Ʈ�� �����ϴ�." << endl;
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
							cout << "����Ʈ�� ��� ���ֽ��ϴ�." << endl;
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
			cout << "���� ������ " << V_num << endl;
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
				cout << "�������� �� ��ǥ��: ";
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
				cout << "�������� �� ��ǥ��: ";
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