#define _CRT_SECURE_NO_WARNINGS


#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <cassert>

const double PI = 3.141592;
//---윈도우 사이즈 변수
int WinSize_r = 1200;
int WinSize_w = 800;
GLfloat changeratio = (GLfloat)WinSize_r / WinSize_w;
using namespace std;
int windowID;		//---윈도우 아이디

//---콜백 함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid TimerFunction(int value);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid mouseWheel(int button, int dir, int x, int y);
GLvoid Motion(int x, int y);

//---glsl
void make_vertexShader();
void make_fragmentShader();
void InitShader();
void InitBuffer();
char* filetobuf(const char* file);
GLchar* vertexsource, * fragmentsource;		//---소스코드 저장 변수
GLuint vertexshader, fragmentshader;		//---세이더 객체
GLuint vao;
GLuint vbo;
//---쉐이더 프로그램
GLuint s_program;

GLfloat* vertex = NULL;

//----점 좌표
int DrawTimer = 50;		//---타이머 주기
int spotcount = 100;		//---원 하나에 찍히는 점의 개수
int drawspot = 0;			//---그려진 점의 개수
GLfloat MaxRadian = 0.2f;	//---큰 원의 반지름
GLfloat pointSize = 5.0f;	//---점의 크기
int Dir = 0;				//---그려지는 방향

GLfloat BackColor[3];

int playcount = 1;

bool mouseclick = false;

//---추가구현 변수
double startx, starty;
int Wheelcount = 10;
bool rightclick = false;

int mousep_x;
int mousep_y;
double Ori_startx, Ori_starty;

bool Play_anim = false;
double seta = 0;
int AnimTimer = 50;		//---타이머 주기
double distance_s;
class Spot
{
private:
	GLfloat x;
	GLfloat y;
	GLfloat z;
public:
	Spot() { x = 0.0f; y = 0.0f; z = 0.0f; }
	~Spot() {}
	void Set(GLfloat a, GLfloat b) { x = a; y = b; z = 0.0f; }
	GLfloat GetX() { return x; }
	GLfloat GetY() { return y; }
	GLfloat GetZ() { return z; }
};

GLfloat* MakeTornado(double a, double b)
{
	GLfloat x = ((GLfloat)(a - (WinSize_r / 2)) / (WinSize_r / 2));
	GLfloat y = ((GLfloat)((WinSize_w / 2) - b) / (WinSize_w / 2));
	GLfloat x2 = x + (MaxRadian) * 2;
	GLfloat y2 = y;


	GLfloat radius = 0;

	GLfloat* arr = new GLfloat[spotcount * 6];

	if(Play_anim == false) seta = 0;
	distance_s = (360.0 * 3) / spotcount;
	if (Dir == 0) distance_s *= -1;		//방향 전환
	double radian = seta * (PI / 180);


	for (int i = 0; i < spotcount * 3; i++)
	{
		if (i % 3 == 0)				//x좌표
		{
			arr[i] = x + radius * cos(radian) ;
		}
		else if (i % 3 == 1)		//y좌표
		{
			arr[i] = (y + radius * sin(radian) * changeratio);

		}	
		else if (i % 3 == 2)		//z좌표
		{
			arr[i] = 0.0f;
			seta += distance_s;
			radian = seta * (PI / 180);
			radius += (MaxRadian) / spotcount;
		}
	}
	for (int i = 0; i < spotcount * 3; i++)
	{
		if (i % 3 == 0)				//x좌표
		{
			arr[spotcount * 3 + i] = x2 + radius * cos(radian) * -1;
		}
		else if (i % 3 == 1)		//y좌표
		{
			arr[spotcount * 3 + i] = (y2 + radius * sin(radian) * changeratio);

		}
		else if (i % 3 == 2)		//z좌표
		{
			arr[spotcount * 3 + i] = 0.0f;
			seta += distance_s;
			radian = seta * (PI / 180);
			radius -= (MaxRadian) / spotcount;
		}
	}
	return arr;
}

void RandRGB()
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<GLclampf> rd_RGB(0.0, 1.0f);
	 
	for (int i = 0; i < 3; i++)
		BackColor[i] = rd_RGB(eng);

}

void main(int argc, char** argv)		//---윈도우 출력, 콜백함수 설정
{
	//---윈도우 생성
	glutInit(&argc, argv);		//glut초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//디스플레이 모드 설정
	glutInitWindowPosition(0, 0);					//윈도우 위치 지정
	glutInitWindowSize(WinSize_r, WinSize_w);					//윈도우 크기 지정
	windowID = glutCreateWindow("Example1");					//윈도우 생성

	//---GLEW 초기화
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)		//glew 초기화에 실패할 경우
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW initialized\n";

	InitShader();
	InitBuffer();

	RandRGB();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMouseWheelFunc(mouseWheel);
	glutMotionFunc(Motion);

	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(BackColor[0], BackColor[1], BackColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (mouseclick == true)
	{
		int vColorLocation = glGetUniformLocation(s_program, "vColor");

		glUseProgram(s_program);
		glUniform4f(vColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);

		//--- 점 그리기
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);

		glPointSize((pointSize));
		//glPointSize(10.0f);

		glDrawArrays(GL_POINTS, 0, drawspot);
		glBindVertexArray(0);
		glDisableVertexAttribArray(0);
	}
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	changeratio = (double)w / (double)h;
	glViewport(0, 0, w, h);
	WinSize_r = w;
	WinSize_w = h;
	vertex = MakeTornado(startx, starty);
	InitBuffer();
	glutPostRedisplay();
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	//--- 버텍스 세이더 객체 만들기
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	//--- 버텍스 세이더 컴파일하기
	glCompileShader(vertexshader);

	//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}
void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");

	//--- 프래그먼트 세이더 객체 만들기
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	//--- 세이더 코드를 세이더 객체에 넣기
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	//--- 프래그먼트 세이더 컴파일
	glCompileShader(fragmentshader);

	//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
	GLint result;
	GLchar errorLog[512];
	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
		return;
	}
}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();

	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);

	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	glLinkProgram(s_program);

	// ---세이더가 잘 연결되었는지 체크하기
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(s_program, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(s_program, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}
	//--- Shader Program 사용하기
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glBufferData(GL_ARRAY_BUFFER, spotcount * 6 * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);

}



GLvoid Mouse(int button, int state, int x, int y)
{
	int ClickNum = 0;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		std::random_device rd;
		std::default_random_engine eng(rd());
		std::uniform_int_distribution<int> rand(0, 1);
		Dir = rand(eng);
		startx = x; starty = y;
		vertex = MakeTornado(startx, starty);

		drawspot = 0;
		RandRGB();
		playcount++;

		mouseclick = true;
		InitBuffer();

		glutTimerFunc(DrawTimer, TimerFunction, playcount);
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		rightclick = true;
		mousep_x = x;
		mousep_y = y;
		Ori_startx = startx;
		Ori_starty = starty;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
		rightclick = false;

	glutPostRedisplay();
}
GLvoid TimerFunction(int value)
{
	if (drawspot < spotcount * 2 && playcount == value)
	{
		drawspot++;
		glutTimerFunc(DrawTimer, TimerFunction, playcount);
	}
	else if (value == 0 && Play_anim == true)
	{
		seta = ((int)seta + 10) % (int)distance_s;
		glutTimerFunc(AnimTimer, TimerFunction, 0);
		vertex = MakeTornado(startx, starty);
		InitBuffer();
	}
	glutPostRedisplay();
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case'q':
		if (vertex != NULL) delete vertex;
		glutDestroyWindow(windowID);
		break;
	case 'a':
		if (Play_anim == true)
		{
			Play_anim = false;
			seta = 0;
			vertex = MakeTornado(startx, starty);
			InitBuffer();
		}
		else
		{
			Play_anim = true;
			glutTimerFunc(AnimTimer, TimerFunction, 0);
		}
		break;
	case'r':
		mouseclick = false;
		drawspot = 0;			//---그려진 점의 개수
		MaxRadian = 0.2f;	//---큰 원의 반지름
		pointSize = 5.0f;	//---점의 크기
		break;
	}
	glutPostRedisplay();
}
GLvoid mouseWheel(int button, int dir, int x, int y)
{
	double disx = ((double)x - startx);
	double disy = ((double)y - starty);
	if (dir > 0)			//---위
	{

		Wheelcount++;
		startx -= disx * 5 / 100;
		starty -= disy * 5 / 100;
		MaxRadian += MaxRadian * 5 / 100;
		pointSize += pointSize * 5 / 100;
	}
	else if (dir < 0)		//---아래
	{
		if (pointSize > 2.0f)
		{
			Wheelcount--;
			startx += disx * 5 / 100;
			starty += disy * 5 / 100;
			MaxRadian -= MaxRadian * 5 / 100;
			pointSize -= pointSize * 5 / 100;
		}
	}
	std::cout << pointSize << endl;
	vertex = MakeTornado(startx, starty);
	//cout << augment << endl;
	InitBuffer();
	glutPostRedisplay();
}
GLvoid Motion(int x, int y)
{
	if (rightclick == true)
	{
		float moveRange_x = x - mousep_x;
		float moveRange_y = mousep_y - y;

		GLfloat GLmoveRange_x = (GLfloat)(moveRange_x / (WinSize_r / 2));
		GLfloat GLmoveRange_y = (GLfloat)(moveRange_y / (WinSize_w / 2));

		//std::cout << GLmoveRange_x << ", " << GLmoveRange_y << std::endl;
		startx = Ori_startx + moveRange_x;
		starty = Ori_starty - moveRange_y;
		vertex = MakeTornado(startx, starty);
		InitBuffer();
	}

	glutPostRedisplay();
}
char* filetobuf(const char* file)
{
	FILE* fp;
	long length;
	char* buf;

	fp = fopen(file, "rb");

	if (!fp)
		assert(fp != nullptr);

	fseek(fp, 0, SEEK_END);
	length = ftell(fp);
	buf = (char*)malloc(sizeof(char) * length + 1);
	fseek(fp, 0, SEEK_SET);
	if (buf != nullptr)
		fread(buf, 1, length + 1, fp);

	fclose(fp);
	if (buf != nullptr)
		buf[length] = 0;

	return buf;
}