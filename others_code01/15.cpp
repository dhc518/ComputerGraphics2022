#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <cassert>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
#include "objReader.h"
#include <cmath>

#define Speed 1

int Tornado_Frame = 20;

//---윈도우 사이즈 변수
int WinSize_r = 1000;
int WinSize_w = 1000;
GLfloat changeratio = (GLfloat)WinSize_r / WinSize_w;
using namespace std;
int windowID;		//---윈도우 아이디

//---콜백 함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);
void SpecialKeyboard(int key, int x, int y);

//---glsl
void make_vertexShader();
void make_fragmentShader();
void InitShader();
void InitBuffer();
char* filetobuf(const char* file);
void ReadObj(FILE* objFile);
GLchar* vertexsource[2], * fragmentsource[2];		//---소스코드 저장 변수
GLuint vertexshader[2], fragmentshader[2];		//---세이더 객체
GLuint vao_line, vao;
GLuint vbo_line, vbo[2];
GLuint ebo;
GLuint vao_tornado, vbo_tornado;
//---쉐이더 프로그램
GLuint s_program;
GLuint s_program_line;

//---선
GLfloat back_line[18] = { -1.0 , 0, 0,
						1.0, 0, 0, 
						0, 1.0, 0, 
						0, -1.0 ,0,
						0, 0, 1.0,
						0, 0, -1.0};

glm::vec3* vertex = NULL;
int* face = NULL;



GLfloat cubeColor[24];

GLUquadricObj* qobj;

int cube_rotate_x = 0;
int cube_rotate_y = 0;

GLfloat CRX = 0;
GLfloat CRY = 0;

int cone_rotate_x = 0;
int cone_rotate_y = 0;

GLfloat CONERX = 0;
GLfloat CONERY = 0;

int line_rotate_y = 0;
GLfloat LRY = 30;

bool changeShape = false;

//---------15번 변수
//----원뿔(좌)
GLfloat L_shape_trans[3] = {-0.5,0.0,0.0};
GLfloat L_shape_scale = 0.1;
GLfloat L_shape_scale_origin = 1.0;

//----육면체(우)
GLfloat R_shape_trans[3] = {0.5,0.0,0.0};
GLfloat R_shape_scale = 0.2;
GLfloat R_shape_scale_origin = 1.0;

GLfloat Line_y_trans = 0.0;

//----토네이도 애니메이션
bool tornado_anim = false;
const double PI = 3.141592;
GLfloat MaxRadian = 1.0f;	//---큰 원의 반지름
int Dir = 0;				//---그려지는 방향
int spotcount = 200;		//---원 하나에 찍히는 점의 개수
double distance_s;
double seta = 0;

int L_tornado = 0;
int R_tornado = 0;


GLfloat* vertex_tornado;


bool MoveToOri = false;
bool changeMove = false;
GLfloat* MakeTornado()
{
	GLfloat x = 0;
	GLfloat z = 0;



	GLfloat radius = 0;

	GLfloat* arr = new GLfloat[spotcount * 6];

	distance_s = (360.0 * 6) / spotcount;
	if (Dir == 0) distance_s *= -1;		//방향 전환
	double radian = seta * (PI / 180);


	for (int i = 0; i < spotcount * 3; i++)
	{
		if (i % 3 == 0)				//x좌표
		{
			arr[i] = x + radius * cos(radian);
		}
		else if (i % 3 == 1)		//y좌표
		{
			arr[i] = 0.0f;
		}
		else if (i % 3 == 2)		//z좌표
		{
			arr[i] = (z + radius * sin(radian) * changeratio);
			seta += distance_s;
			radian = seta * (PI / 180);
			radius += (MaxRadian) / spotcount;
		}
	}
	return arr;
}

class PointToPoint
{
private:
	GLfloat p1[3];
	GLfloat p2[3];
	int t;
	bool dir = true;
public:
	PointToPoint()
	{
		for (int i = 0; i < 3; i++)
		{
			p1[i] = 0; p2[i] = 0;
		}
		t = 0;
	}
	PointToPoint(GLfloat gp1[3], GLfloat gp2[3]) {
		for (int i = 0; i < 3; i++)
		{
			p1[i] = gp1[i]; p2[i] = gp2[i];
		}
		t = 0;
	}
	GLfloat GetPoint(char c)
	{
		int a;
		switch (c)
		{
		case 'x':
			a = 0;
			break;
		case 'y':
			a = 1;
			break;
		case 'z':
			a = 2;
			break;
		}
		return (1 - (float)t / 100) * p1[a] + (float)t / 100 * p2[a];
	}
	void SetT() { t = 0; }
	bool PlusT(int a) 
	{
		if (t < 100)
		{
			t += a;
			return true;
		}
		else
		{
			t = 0;
			return false;
		}
	}
	void SetP(GLfloat gp1[3], GLfloat gp2[3])
	{
		for (int i = 0; i < 3; i++)
		{
			p1[i] = gp1[i]; p2[i] = gp2[i];
		}
		t = 0;
	}
	void change()
	{
		//if (dir == true) dir = false;
		//else dir = true;
		GLfloat a;
		for (int i = 0; i < 3; i++)
		{
			a = p1[i];
			p1[i] = p2[i];
			p2[i] = a;
		}
	}
};

PointToPoint Rmove_T;
PointToPoint Lmove_T;
PointToPoint Rtornado_T;
PointToPoint Ltornado_T;
PointToPoint RChanmove_T;
PointToPoint LChanmove_T;
GLfloat L_Change_Array[3][3] = { 0 };
GLfloat R_Change_Array[3][3] = { 0 };


void RandRGB()
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<GLclampf> rd_RGB(0.0, 1.0f);

	for (int i = 0; i < 24; i++)
	{
		cubeColor[i] = rd_RGB(eng);
	}
}

void main(int argc, char** argv)		//---윈도우 출력, 콜백함수 설정
{
	//---윈도우 생성
	glutInit(&argc, argv);		//glut초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	//디스플레이 모드 설정
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

	//glEnable(GL_DEPTH_TEST);
	RandRGB();

	vertex_tornado = MakeTornado();

	InitShader();
	InitBuffer();

	cout << "a,b,c/A,B,C: 원뿔 이동" << endl;
	cout << "x,y,z/X,Y,Z: 육면체 이동" << endl;
	cout << "w/W: 원뿔 제자리 신축" << endl;
	cout << "e/E: 육면체 제자리 신축" << endl;
	cout << "d/D: 원뿔 원점 기준 신축" << endl;
	cout << "f/F: 육면체 원점 기준 신축" << endl;
	cout << "r: 토네이도 애니메이션" << endl;
	cout << "t: 원점 이동 애니메이션" << endl;
	cout << "s: 자리 바꾸기 애니메이션" << endl;
	cout << "위/아래 화살표키: 전체 도형의 이동" << endl;


	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);

	glutMainLoop();
}
GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//-------------선 그리기
	int vColorLocation_line = glGetUniformLocation(s_program_line, "vColor");
	unsigned int transformLocation = glGetUniformLocation(s_program_line, "model");

	glUseProgram(s_program_line);

	glm::mat4 TR_line = glm::mat4(1.0f);
	TR_line = glm::rotate(TR_line, (GLfloat)glm::radians(30.0), glm::vec3(1.0, 0.0, 0.0));
	TR_line = glm::rotate(TR_line, (GLfloat)glm::radians(LRY), glm::vec3(0.0, 1.0, 0.0));

	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(TR_line));

	glBindVertexArray(vao_line);
	
	glUniform4f(vColorLocation_line, 0.0f, 1.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 2, 2);	//----y축
	
	TR_line = glm::translate(TR_line, glm::vec3(0, Line_y_trans, 0));
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(TR_line));
	glEnableVertexAttribArray(0);
	glUniform4f(vColorLocation_line, 1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 0, 2);	//----x축

	glUniform4f(vColorLocation_line, 0.0f, 0.0f, 1.0f, 1.0f);
	glDrawArrays(GL_LINES, 4, 2);	//----z축

		//----토네이도 
	if (tornado_anim == true)
	{
		glUniform4f(vColorLocation_line, 0.1f, 0.7f, 0.0f, 1.0f);
		glBindVertexArray(vao_tornado);
		glDrawArrays(GL_LINE_STRIP, 0, spotcount);

	}

	//---------- 원뿔 그리기(좌)
	glUniform4f(vColorLocation_line, 0.0f, 0.0f, 0.0f, 1.0f);

	glm::mat4 TR_cylinder = glm::mat4(1.0f);
	glm::mat4 T_cylinder = glm::mat4(1.0f);
	glm::mat4 Rx_cylinder = glm::mat4(1.0f);
	glm::mat4 Ry_cylinder = glm::mat4(1.0f);
	glm::mat4 S_cylinder = glm::mat4(1.0f);
	glm::mat4 OS_cylinder = glm::mat4(1.0f);


	if (MoveToOri == true)
		T_cylinder = glm::translate(T_cylinder, glm::vec3(Lmove_T.GetPoint('x'), Lmove_T.GetPoint('y'), Lmove_T.GetPoint('z')));

	else if (tornado_anim == true)
		T_cylinder = glm::translate(T_cylinder, glm::vec3(Ltornado_T.GetPoint('x'), Ltornado_T.GetPoint('y'), Ltornado_T.GetPoint('z')));

	else if (changeMove == true)
		T_cylinder = glm::translate(T_cylinder, glm::vec3(LChanmove_T.GetPoint('x'), LChanmove_T.GetPoint('y'), LChanmove_T.GetPoint('z')));

	else
		T_cylinder = glm::translate(T_cylinder, glm::vec3(L_shape_trans[0], L_shape_trans[1], L_shape_trans[2]));

	Rx_cylinder = glm::rotate(Rx_cylinder, (GLfloat)glm::radians(CONERX), glm::vec3(1.0, 0.0, 0.0));
	Ry_cylinder = glm::rotate(Ry_cylinder, (GLfloat)glm::radians(CONERY), glm::vec3(0.0, 1.0, 0.0));
	S_cylinder = glm::scale(S_cylinder, glm::vec3(L_shape_scale, L_shape_scale, L_shape_scale));
	OS_cylinder = glm::scale(OS_cylinder, glm::vec3(L_shape_scale_origin, L_shape_scale_origin, L_shape_scale_origin));

	TR_cylinder = OS_cylinder * TR_line * T_cylinder * Rx_cylinder * Ry_cylinder * S_cylinder;

	transformLocation = glGetUniformLocation(s_program_line, "model");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(TR_cylinder));
	qobj = gluNewQuadric(); // 객체 생성하기
	gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
	gluQuadricNormals(qobj, GLU_SMOOTH);
	gluQuadricOrientation(qobj, GLU_OUTSIDE);

	gluCylinder(qobj, 0.8, 0.0, 2.0, 20, 8);





	//---------- 육면체 그리기(우)


	glm::mat4 TR_cube = glm::mat4(1.0f);
	glm::mat4 T_cube = glm::mat4(1.0f);
	glm::mat4 Rx_cube = glm::mat4(1.0f);
	glm::mat4 Ry_cube = glm::mat4(1.0f);
	glm::mat4 S_cube = glm::mat4(1.0f);
	glm::mat4 OS_cube = glm::mat4(1.0f);

	if(MoveToOri == true)
		T_cube = glm::translate(T_cube, glm::vec3(Rmove_T.GetPoint('x'), Rmove_T.GetPoint('y'), Rmove_T.GetPoint('z')));
	else if(tornado_anim == true)
		T_cube = glm::translate(T_cube, glm::vec3(Rtornado_T.GetPoint('x'), Rtornado_T.GetPoint('y'), Rtornado_T.GetPoint('z')));
	else if(changeMove == true)
		T_cube = glm::translate(T_cube, glm::vec3(RChanmove_T.GetPoint('x'), RChanmove_T.GetPoint('y'), RChanmove_T.GetPoint('z')));
	else
		T_cube = glm::translate(T_cube, glm::vec3(R_shape_trans[0], R_shape_trans[1], R_shape_trans[2]));
	Rx_cube = glm::rotate(Rx_cube, (GLfloat)glm::radians(CRX), glm::vec3(1.0, 0.0, 0.0));
	Ry_cube = glm::rotate(Ry_cube, (GLfloat)glm::radians(CRY), glm::vec3(0.0, 1.0, 0.0));
	S_cube = glm::scale(S_cube, glm::vec3(R_shape_scale, R_shape_scale, R_shape_scale));
	OS_cube = glm::scale(OS_cube, glm::vec3(R_shape_scale_origin, R_shape_scale_origin, R_shape_scale_origin));

	TR_cube = OS_cube * TR_line * T_cube * Rx_cube * Ry_cube * S_cube;



	glUseProgram(s_program);

	glBindVertexArray(vao);
	transformLocation = glGetUniformLocation(s_program, "modelTransform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(TR_cube));
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0));





	glBindVertexArray(0);

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void make_vertexShader()
{
	vertexsource[0] = filetobuf("vertex13.glsl");
	vertexsource[1] = filetobuf("vertex_15.glsl");

	for (int i = 0; i < 2; i++)
	{
		//--- 버텍스 세이더 객체 만들기
		vertexshader[i] = glCreateShader(GL_VERTEX_SHADER);

		//--- 세이더 코드를 세이더 객체에 넣기
		glShaderSource(vertexshader[i], 1, (const GLchar**)&vertexsource[i], 0);

		//--- 버텍스 세이더 컴파일하기
		glCompileShader(vertexshader[i]);

		//--- 컴파일이 제대로 되지 않은 경우: 에러 체크
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(vertexshader[i], GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(vertexshader[i], 512, NULL, errorLog);
			std::cerr << "ERROR: vertex shader 컴파일 실패\n" << errorLog << std::endl;
			return;
		}
	}
	
}
void make_fragmentShader()
{
	fragmentsource[0] = filetobuf("fragment13.glsl");
	fragmentsource[1] = filetobuf("fragment_15.glsl");

	for (int i = 0; i < 2; i++)
	{

		//--- 프래그먼트 세이더 객체 만들기
		fragmentshader[i] = glCreateShader(GL_FRAGMENT_SHADER);

		//--- 세이더 코드를 세이더 객체에 넣기
		glShaderSource(fragmentshader[i], 1, (const GLchar**)&fragmentsource[i], 0);

		//--- 프래그먼트 세이더 컴파일
		glCompileShader(fragmentshader[i]);

		//--- 컴파일이 제대로 되지 않은 경우: 컴파일 에러 체크
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(fragmentshader[i], GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(fragmentshader[i], 512, NULL, errorLog);
			std::cerr << "ERROR: fragment shader 컴파일 실패\n" << errorLog << std::endl;
			return;
		}
	}

}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	s_program_line = glCreateProgram();

	glAttachShader(s_program, vertexshader[0]);
	glAttachShader(s_program, fragmentshader[0]);

	glAttachShader(s_program_line, vertexshader[1]);
	glAttachShader(s_program_line, fragmentshader[1]);

	//--- 세이더 삭제하기
	for (int i = 0; i < 2; i++)
	{
		glDeleteShader(vertexshader[i]);
		glDeleteShader(fragmentshader[i]);
	}

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
	glLinkProgram(s_program_line);
	// ---세이더가 잘 연결되었는지 체크하기
	glGetProgramiv(s_program_line, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(s_program_line, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}
	//--- Shader Program 사용하기
}

void InitBuffer()
{
	//--------------선 그리기
	glGenVertexArrays(1, &vao_line);
	glGenBuffers(1, &vbo_line);
	glBindVertexArray(vao_line);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_line);

	glBufferData(GL_ARRAY_BUFFER,  18 * sizeof(GLfloat), back_line, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);




	//----------------도형

	obj objfile("cube_15.obj");
	objfile.ReadObj();

	glGenVertexArrays(1,& vao);
	glGenBuffers(2, vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, objfile.vertexNum * 3 * sizeof(GLfloat), objfile.vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objfile.faceNum * 3 * sizeof(int), objfile.face_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cubeColor, GL_STATIC_DRAW);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


	//---토네이도
	glGenVertexArrays(1, &vao_tornado);
	glGenBuffers(1, &vbo_tornado);

	glBindVertexArray(vao_tornado);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_tornado);
	glBufferData(GL_ARRAY_BUFFER, spotcount * 3 * sizeof(GLfloat), vertex_tornado, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}
bool tornado_dir[2] = { false };

int L_index = 0, R_index = 0;

GLvoid TimerFunction(int value)
{
	if (value == 1 && tornado_anim == true)
	{
		if (Ltornado_T.PlusT(Tornado_Frame) == false)
		{
			if (tornado_dir[0] == false)
			{
				L_tornado++;
				GLfloat a[3] = { vertex_tornado[L_tornado * 3], vertex_tornado[L_tornado * 3 + 1],vertex_tornado[L_tornado * 3 + 2] };
				GLfloat b[3] = { vertex_tornado[L_tornado * 3+3], vertex_tornado[L_tornado * 3 + 4],vertex_tornado[L_tornado * 3 + 5] };
				Ltornado_T.SetP(a, b);
			}
			else
			{
				L_tornado--;
				GLfloat a[3] = { vertex_tornado[L_tornado * 3], vertex_tornado[L_tornado * 3 + 1],vertex_tornado[L_tornado * 3 + 2] };
				GLfloat b[3] = { vertex_tornado[L_tornado * 3 - 3], vertex_tornado[L_tornado * 3 - 2],vertex_tornado[L_tornado * 3 - 1] };

				Ltornado_T.SetP(a, b);
			}
		}
		if (Rtornado_T.PlusT(Tornado_Frame) == false)
		{
			if (tornado_dir[1] == false)
			{
				R_tornado--;
				GLfloat a[3] = { vertex_tornado[R_tornado * 3], vertex_tornado[R_tornado * 3 + 1],vertex_tornado[R_tornado * 3 + 2] };
				GLfloat b[3] = { vertex_tornado[R_tornado * 3 - 3], vertex_tornado[R_tornado * 3 - 2],vertex_tornado[R_tornado * 3 - 1] };

				Rtornado_T.SetP(a, b);
			}
			else
			{
				R_tornado++;
				GLfloat a[3] = { vertex_tornado[R_tornado * 3], vertex_tornado[R_tornado * 3 + 1],vertex_tornado[R_tornado * 3 + 2] };
				GLfloat b[3] = { vertex_tornado[R_tornado * 3 + 3], vertex_tornado[R_tornado * 3 + 4],vertex_tornado[R_tornado * 3 + 5] };

				
				Rtornado_T.SetP(a, b);
			}
		}

		if (L_tornado == 1) tornado_dir[0] = false;
		else if(L_tornado == spotcount -2) tornado_dir[0] = true;
		if (R_tornado == 1) tornado_dir[1] = true;
		else if (R_tornado == spotcount - 2) tornado_dir[1] = false;

		glutTimerFunc(10, TimerFunction, 1);
	}
	else if (value == 2 && MoveToOri == true)
	{
		if (Rmove_T.PlusT(1) == false) Rmove_T.change();
		if (Lmove_T.PlusT(1) == false)Lmove_T.change();
		glutTimerFunc(20, TimerFunction, 2);

	}
	else if (value == 3 && changeMove == true)
	{
		if (LChanmove_T.PlusT(1) == false)
		{
			L_index = (L_index + 1) % 3;
			LChanmove_T.SetP(L_Change_Array[L_index], L_Change_Array[(L_index + 1) % 3]);
		}
		if (RChanmove_T.PlusT(1) == false)
		{
			R_index = (R_index + 1) % 3;
			RChanmove_T.SetP(R_Change_Array[R_index], R_Change_Array[(R_index + 1) % 3]);
		}
		glutTimerFunc(20, TimerFunction, 3);

	}
	glutPostRedisplay();
}



GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
		//---좌측 도형의 이동
	case 'a':
		L_shape_trans[0] += 0.05;
		break;
	case 'b':
		L_shape_trans[1] += 0.05;
		break;
	case 'c':
		L_shape_trans[2] += 0.05;
		break;
	case 'A':
		L_shape_trans[0] -= 0.05;
		break;
	case 'B':
		L_shape_trans[1] -= 0.05;
		break;
	case 'C':
		L_shape_trans[2] -= 0.05;
		break;
		//---우측 도형의 이동
	case 'x':
		R_shape_trans[0] += 0.05;
		break;
	case 'y':
		R_shape_trans[1] += 0.05;
		break;
	case 'z':
		R_shape_trans[2] += 0.05;
		break;
	case 'X':
		R_shape_trans[0] -= 0.05;
		break;
	case 'Y':
		R_shape_trans[1] -= 0.05;
		break;
	case 'Z':
		R_shape_trans[2] -= 0.05;
		break;
		//---좌측 도형 제자리 신축
	case 'w':
		if(L_shape_scale < 0.5) L_shape_scale += 0.01;
		break;
	case 'W':
		if (L_shape_scale > 0.05) L_shape_scale -= 0.01;
		break;
		//---우측 도형 제자리 신축
	case 'e':
		if (R_shape_scale < 0.5) R_shape_scale += 0.01;
		break;
	case 'E':
		if (R_shape_scale > 0.05) R_shape_scale -= 0.01;
		break;
	case 'd':
		if (L_shape_scale_origin < 1.5) L_shape_scale_origin += 0.01;
		break;
	case 'D':
		if (L_shape_scale_origin > 0.5) L_shape_scale_origin -= 0.01;
		break;
		//---우측 도형 제자리 신축
	case 'f':
		if (R_shape_scale_origin < 1.5) R_shape_scale_origin += 0.01;
		break;
	case 'F':
		if (R_shape_scale_origin > 0.5) R_shape_scale_origin -= 0.01;
		break;
	case 'q':
		glutDestroyWindow(windowID);
		break;
	case 'r':
		if (tornado_anim == false)
		{
			int spot[2] = {0};
			float Min_range[2] = { 0 };
			float range = 0;

			for (int i = 0; i < spotcount; i++)
			{
				range = pow((float)L_shape_trans[0] - vertex_tornado[i * 3], 2) +
					pow((float)L_shape_trans[1] - vertex_tornado[i * 3 + 1], 2) +
					pow((float)L_shape_trans[2] - vertex_tornado[i * 3 + 2], 2);
				if (range < Min_range[0] || i == 0)
				{
					Min_range[0] = range;
					spot[0] = i;

				}
				range = pow((float)R_shape_trans[0] - vertex_tornado[i * 3], 2) +
					pow((float)R_shape_trans[1] - vertex_tornado[i * 3 + 1], 2) +
					pow((float)R_shape_trans[2] - vertex_tornado[i * 3 + 2], 2);
				if (range < Min_range[1] || i == 0)
				{

					Min_range[1] = range;
					spot[1] = i;

				}
			}

			L_tornado = spot[0];
			R_tornado = spot[1];
			GLfloat a[3] = { vertex_tornado[L_tornado * 3], vertex_tornado[L_tornado * 3 + 1],vertex_tornado[L_tornado * 3 + 2] };
			GLfloat b[3] = { vertex_tornado[L_tornado * 3 + 3], vertex_tornado[L_tornado * 3 + 4],vertex_tornado[L_tornado * 3 + 5] };

			Ltornado_T.SetP(a, b);
			GLfloat a2[3] = { vertex_tornado[R_tornado * 3], vertex_tornado[R_tornado * 3 + 1],vertex_tornado[R_tornado * 3 + 2] };
			GLfloat b2[3] = { vertex_tornado[R_tornado * 3 - 3], vertex_tornado[R_tornado * 3 - 2],vertex_tornado[R_tornado * 3 - 1] };

			Rtornado_T.SetP(a2, b2);

			MoveToOri = false;
			changeMove = false;

			tornado_anim = true;
		}
		else
		{
			tornado_anim = false;
		}
		glutTimerFunc(50, TimerFunction, 1);
		break;

	case 't':
		if (MoveToOri == false)
		{
			tornado_anim = false;
			changeMove = false;

			MoveToOri = true;
			GLfloat a[3] = { 0,0,0 };
			Rmove_T.SetP(R_shape_trans, a);
			Lmove_T.SetP(L_shape_trans, a);
			glutTimerFunc(50, TimerFunction, 2);
		}
		else MoveToOri = false;
		

		break;
	case 's':
		if (changeMove == false)
		{
			changeMove = true;
			MoveToOri = false;
			tornado_anim = false;
			
			for (int i = 0; i < 3; i++)
			{
				L_Change_Array[0][i] = L_shape_trans[i];
				L_Change_Array[2][i] = R_shape_trans[i];

				R_Change_Array[0][i] = R_shape_trans[i];
				R_Change_Array[2][i] = L_shape_trans[i];
			}
			L_Change_Array[1][0] = (L_shape_trans[0] + R_shape_trans[0]) / 2;
			L_Change_Array[1][1] = (L_shape_trans[1] + R_shape_trans[1]) / 2 + 0.3;
			L_Change_Array[1][2] = (L_shape_trans[2] + R_shape_trans[2]) / 2 + 0.7;
			R_Change_Array[1][0] = (L_shape_trans[0] + R_shape_trans[0]) / 2;
			R_Change_Array[1][1] = (L_shape_trans[1] + R_shape_trans[1]) / 2 + 0.3;
			R_Change_Array[1][2] = (L_shape_trans[2] + R_shape_trans[2]) / 2 - 0.7;
			RChanmove_T.SetP(R_Change_Array[0], R_Change_Array[1]);
			LChanmove_T.SetP(L_Change_Array[0], L_Change_Array[1]);
			L_index = 0, R_index = 0;
			glutTimerFunc(30, TimerFunction, 3);
		}
		else
		{
			changeMove = false;

		}
		break;
	case'+':
		if (Tornado_Frame<90)Tornado_Frame += 10;
		break;
	case'-':
		if (Tornado_Frame > 10)Tornado_Frame -= 10;
		break;
	case'h':
		cout << "a,b,c/A,B,C: 원뿔 이동" << endl;
		cout << "x,y,z/X,Y,Z: 육면체 이동" << endl;
		cout << "w/W: 원뿔 제자리 신축" << endl;
		cout << "e/E: 육면체 제자리 신축" << endl;
		cout << "d/D: 원뿔 원점 기준 신축" << endl;
		cout << "f/F: 육면체 원점 기준 신축" << endl;
		cout << "r: 토네이도 애니메이션" << endl;
		cout << "t: 원점 이동 애니메이션" << endl;
		cout << "s: 자리 바꾸기 애니메이션" << endl;
		cout << "위/아래 화살표키: 전체 도형의 이동" << endl;
		break;
	}

	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{

	case GLUT_KEY_UP:
		Line_y_trans += 0.05;
		break;
	case GLUT_KEY_DOWN:
		Line_y_trans -= 0.05;
		break;
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