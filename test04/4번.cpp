#include <iostream>
#include <random>

using namespace std;
class point
{
private:
	int x;
	int y;
public:
	void setPoint(int a, int b)
	{
		x = a; y = b;
	}	
	void setXPoint(int a)
	{
		x = a;
	}
	void setYPoint(int a)
	{
		y = a;
	}
	void pritPoint()
	{
		cout << "(" << x << ", " << y << ")";
	}
	int getx() { return x; }
	int gety() { return y; }
	void setx(int n) { x = n; }
	void sety(int n) { y = n; }
};

class Rect
{
private:
	point p[2];
public:
	Rect() 
	{
		int n;
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dis1(0, 800);
		uniform_int_distribution<int> dis2(0, 600);
		for (int i = 0; i < 2; i++)
		{
			p[i].setPoint(dis1(gen), dis2(gen));
		}
		if (p[0].getx() > p[1].getx())
		{
			int n;
			n = p[0].getx();
			p[0].setx(p[1].getx());
			p[1].setx(n);
		}
		if (p[0].gety() > p[1].gety())
		{
			int n;
			n = p[0].gety();
			p[0].sety(p[1].gety());
			p[1].sety(n);
		}
	}
	~Rect() {}

	void R_Print()
	{
		for (int i = 0; i < 2; i++)
		{
			p[i].pritPoint();
			cout << " ";
		}
		cout << endl;
	}
	point Getp0(){return p[0];}
	point Getp1() { return p[1]; }
	void MoveRec(int a, int b)
	{
		bool er = false;
		for (int i = 0; i < 2; i++)
		{
			if ((p[i].getx() + a) > 800 || (p[i].getx() + a) < 0)
				er = true;
			else if ((p[i].gety() + b) > 600 || (p[i].gety() + b) < 0)
				er = true;
		}
		if (er == true)
			cout << "error!!" << endl;
		else
		{
			for (int i = 0; i < 2; i++)
			{
				p[i].setx(p[i].getx() + a);
				p[i].sety(p[i].gety() + b);
			}
		}

	}
};
bool InRect(Rect a, point p)
{
	point a_p1 = a.Getp0();
	point a_p2 = a.Getp1();
	if (a_p1.getx() < p.getx() && a_p2.getx() > p.getx()
		&& a_p1.gety() < p.gety() && a_p2.gety() > p.gety())
		return true;
	else
		return false;
}
bool RectByRect(Rect a, Rect b)		//점이 사각형 안에 있지 않고 사각형끼리 부딫힐때
{
	point a_p1 = a.Getp0();
	point a_p2 = a.Getp1();
	point b_p1 = b.Getp0();
	point b_p2 = b.Getp1();
	if (a_p1.getx() > b_p1.getx() && a_p1.getx() < b_p2.getx() && b_p1.gety() > a_p2.gety() && b_p1.gety() < a_p1.gety())
		return true;
	else if (b_p1.getx() > a_p1.getx() && b_p1.getx() < a_p2.getx() && a_p1.gety() > b_p2.gety() && a_p1.gety() < b_p1.gety())
		return true;

	else
		return false;
}
void CrashCheck(Rect a, Rect b)
{
	point a_p1 = a.Getp0();
	point a_p2 = a.Getp1();
	point b_p1 = b.Getp0();
	point b_p2 = b.Getp1();
	if (InRect(a, b_p1) || InRect(a, b_p2) || InRect(b, a_p1) || InRect(b, a_p2) || RectByRect(a,b))
		cout << "Rectangle 1 & Rectangle 2 collide!!" << endl;

}

int main()
{
	Rect r[2];
	char c;
	int changer = 0;
	

	while (1)
	{
		for (int i = 0; i < 2; i++)
		{
			cout << "Rect " << i + 1 << ": ";
			r[i].R_Print();
		}
		CrashCheck(r[0], r[1]);

		cout << "Command(wasd): ";
		cin >> c;

		switch (c)
		{
		case 'w':
			r[changer].MoveRec(0, 50);
			break;
		case 'a':
			r[changer].MoveRec(-50, 0);
			break;
		case 's':
			r[changer].MoveRec(0, -50);
			break;
		case 'd':
			r[changer].MoveRec(50, 0);
			break;
		case 'q':
			return 0;
			break;
		}
		changer = (changer + 1) % 2;
	}
	return 0;
}