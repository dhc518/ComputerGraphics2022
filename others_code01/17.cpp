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

using namespace std;

//---윈도우 사이즈 변수
int WinSize_r = 1000;
int WinSize_w = 1000;
int windowID;		//---윈도우 아이디

//---사용자 정의 함수
GLfloat* MakeCircle(GLfloat radius, int spotcount);

//---콜백 함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

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

//---구 모델
GLUquadricObj* qobj;

//---bool 변수
bool isOrtho = false;
int IsRotate = 0;

//---원 vertex
int spotcount = 1000;		//원의 점 개수
GLfloat* circle_vertex = NULL;

int Smodel_index[3] = { 0, 50, 150 };
int Smodel2_index[3] = { 5,12,15 };
GLfloat Circle_Radian[3] = { 0,-45,45 };

//---행성 색
glm::vec3 middle_sphere_color[3];
glm::vec3 small_sphere_color[3];

//---가운데 구 모델링 값
glm::vec3 BigSphere_trans = glm::vec3(0.0, 0.0, 0.0);
GLfloat BigSphere_rotate = 0.0;

void RandRGB()
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<GLclampf> rd_RGB(0.0, 1.0f);

	for (int i = 0; i < 3; i++)
	{
		middle_sphere_color[i] = glm::vec3(rd_RGB(eng), rd_RGB(eng), rd_RGB(eng));
		small_sphere_color[i] = glm::vec3(rd_RGB(eng), rd_RGB(eng), rd_RGB(eng));
	}
}


GLfloat* MakeCircle(GLfloat radius, int spotcount)
{
	double PI = 3.141592;
	GLfloat x = 0;
	GLfloat z = 0;

	GLfloat seta = 0;

	GLfloat* arr = new GLfloat[spotcount * 3];
	GLfloat distance_s = 360.0 / spotcount;
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
			arr[i] = (z + radius * sin(radian));
			seta += distance_s;
			radian = seta * (PI / 180);
		}
		cout << arr[i] << ", ";
		if (i % 3 == 2) cout << endl;
	}
	return arr;
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


	if (circle_vertex != NULL) delete circle_vertex;
	circle_vertex = MakeCircle(5.0f, spotcount);


	RandRGB();
	InitShader();
	InitBuffer();

	qobj = gluNewQuadric(); // 객체 생성하기
	gluQuadricDrawStyle(qobj, GLU_FILL); // 도형 스타일
	gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
	gluQuadricOrientation(qobj, GLU_OUTSIDE);

	glEnable(GL_DEPTH_TEST);


	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutTimerFunc(10, TimerFunction, 0);

	glutMainLoop();
}
GLvoid drawScene()
{
	//---배경 초기화
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//---카메라 설정
	glm::vec3 cameraPos = glm::vec3(0.0f, -1.2f, 14.0f);		 //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);		 //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLoc_shape = glGetUniformLocation(s_program, "view"); //--- 뷰잉 변환 설정

	//---투영변환
	glm::mat4 projection = glm::mat4(1.0f);
	if(isOrtho == true)
		projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -5.0f, 50.0f);
	else
	{
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- 공간을 약간 뒤로 미뤄줌
	}


	unsigned int projLoc_shape = glGetUniformLocation(s_program, "projection");



	//---------- 구 그리기
	glUseProgram(s_program);
	glUniformMatrix4fv(viewLoc_shape, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc_shape, 1, GL_FALSE, &projection[0][0]);

	int vColorLocation = glGetUniformLocation(s_program, "vColor");
	unsigned int modelLoc = glGetUniformLocation(s_program, "model");

	glm::mat4 Model_sphere = glm::mat4(1.0f);

	Model_sphere = glm::translate(Model_sphere, glm::vec3(BigSphere_trans.x, BigSphere_trans.y, BigSphere_trans.z));
	Model_sphere = glm::rotate(Model_sphere, (GLfloat)glm::radians(0.0), glm::vec3(1.0, 0.0, 0.0));
	Model_sphere = glm::rotate(Model_sphere, (GLfloat)glm::radians(BigSphere_rotate), glm::vec3(0.0, 1.0, 0.0));
	Model_sphere = glm::rotate(Model_sphere, (GLfloat)glm::radians(0.0), glm::vec3(0.0, 0.0, 1.0));
	Model_sphere = glm::scale(Model_sphere, glm::vec3(1.0, 1.0, 1.0));

	glUniform4f(vColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model_sphere));
	gluSphere(qobj, 1.5, 50, 50);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0 * 6));

	//-------------------------------------------------원 그리기(평면)-------------------------------------------------------



	for (int i = 0; i < 3; i++)
	{
		glm::mat4 Model_circleline1 = glm::mat4(1.0f);
		Model_circleline1 = glm::translate(Model_circleline1, glm::vec3(0, 0, 0));
		Model_circleline1 = glm::rotate(Model_circleline1, (GLfloat)glm::radians(Circle_Radian[i]), glm::vec3(0.0, 0.0, 1.0));
		Model_circleline1 = glm::scale(Model_circleline1, glm::vec3(1.0, 1.0, 1.0));
		Model_circleline1 = Model_sphere * Model_circleline1;
		glUniform4f(vColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model_circleline1));

		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_STRIP, 0, spotcount);

		//--------구(원 위 중간 구)

		glm::vec4 OnCircle = glm::vec4(circle_vertex[Smodel_index[i] * 3], circle_vertex[Smodel_index[i] * 3 + 1], circle_vertex[Smodel_index[i] * 3 + 2], 1.0f);
		glm::mat4 Model_sphere_inlargeline = glm::mat4(1.0f);
		Model_sphere_inlargeline = glm::translate(Model_sphere_inlargeline, glm::vec3(OnCircle.x, OnCircle.y, OnCircle.z));
		Model_sphere_inlargeline = glm::scale(Model_sphere_inlargeline, glm::vec3(0.3, 0.3, 0.3));
		Model_sphere_inlargeline = Model_circleline1 * Model_sphere_inlargeline;
		glUniform4f(vColorLocation, middle_sphere_color[i].x, middle_sphere_color[i].y, middle_sphere_color[i].z, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model_sphere_inlargeline));
		gluSphere(qobj, 1.5, 50, 50);

		glm::mat4 Model_circle_smallline1 = glm::mat4(1.0f);
		Model_circle_smallline1 = glm::rotate(Model_circle_smallline1, (GLfloat)glm::radians(-1*Circle_Radian[i]), glm::vec3(0.0, 0.0, 1.0));
		Model_circle_smallline1 = Model_sphere_inlargeline * Model_circle_smallline1;
		glUniform4f(vColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model_circle_smallline1));

		glBindVertexArray(vao);
		glDrawArrays(GL_LINE_STRIP, 0, spotcount);

		//--------가장 작은 구

		//glm::vec4 OnCircle2 = Model_circle_smallline1 * glm::vec4(circle_vertex[Smodel2_index[i] * 3], circle_vertex[Smodel2_index[i] * 3 + 1], circle_vertex[Smodel2_index[i] * 3 + 2], 1.0f);
		glm::vec4 OnCircle2 = glm::vec4(circle_vertex[Smodel2_index[i] * 3], circle_vertex[Smodel2_index[i] * 3 + 1], circle_vertex[Smodel2_index[i] * 3 + 2], 1.0f);
		glm::mat4 Model_sphere_insmallline = glm::mat4(1.0f);
		Model_sphere_insmallline = glm::translate(Model_sphere_insmallline, glm::vec3(OnCircle2.x, OnCircle2.y, OnCircle2.z));
		Model_sphere_insmallline = glm::scale(Model_sphere_insmallline, glm::vec3(0.5, 0.5, 0.5));
		Model_sphere_insmallline = Model_circle_smallline1 * Model_sphere_insmallline;
		glUniform4f(vColorLocation, small_sphere_color[i].x, small_sphere_color[i].y, small_sphere_color[i].z, 1.0f);
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model_sphere_insmallline));
		gluSphere(qobj, 1.5, 50, 50);
	}


	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

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
}

void InitBuffer()
{
	//----------------원 
	glGenVertexArrays(1,&vao);
	glGenBuffers(1, &vbo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, spotcount * 3 * sizeof(GLfloat), circle_vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
}


bool FrontSideAinm_Open = true;
GLfloat LeftRightSideAnimTrans = 0;

GLvoid TimerFunction(int value)
{
	if (value == 0)
	{
		for (int i = 0; i < 3; i++)
		{
			Smodel_index[i] = (Smodel_index[i] + i + 1) % (spotcount);
			Smodel2_index[i] = (Smodel2_index[i] + i + 2) % (spotcount);
		}

		glutTimerFunc(10, TimerFunction, 0);
	}
	if (value == 1 && IsRotate != 0)
	{
		BigSphere_rotate += IsRotate * 0.5;
		glutTimerFunc(10, TimerFunction, 1);
	}
	glutPostRedisplay();
}



GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'p':
		isOrtho = true;
		break;
	case 'P':
		isOrtho = false;
		break;
	case 'm':
		gluQuadricDrawStyle(qobj, GLU_FILL); // 도형 스타일
		break;
	case'M':
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		break;
	case 'w':
		if (BigSphere_trans.y < 5.0f) BigSphere_trans.y += 0.5f;
		break;
	case 'a':
		if (BigSphere_trans.x > -5.0f) BigSphere_trans.x -= 0.5f;
		break;
	case 's':
		if (BigSphere_trans.y > -5.0f) BigSphere_trans.y -= 0.5f;
		break;
	case 'd':
		if (BigSphere_trans.x < 5.0f) BigSphere_trans.x += 0.5f;
		break;
	case 'z':
		if (BigSphere_trans.z < 5.0f) BigSphere_trans.z += 0.5f;
		break;
	case 'x':
		if (BigSphere_trans.z > -20.0f) BigSphere_trans.z -= 0.5f;
		break;
	case 'y':
		if (IsRotate == 1) IsRotate = 0;
		else
		{
			if (IsRotate == 0) glutTimerFunc(10, TimerFunction, 1);
			IsRotate = 1;
		}
		break;
	case 'Y':
		if (IsRotate == -1) IsRotate = 0;
		else
		{
			if (IsRotate == 0) glutTimerFunc(10, TimerFunction, 1);
			IsRotate = -1;
		}

		break;
	case 'q':
		glutDestroyWindow(windowID);
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