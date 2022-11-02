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
GLchar* vertexsource[3], * fragmentsource[3];		//---소스코드 저장 변수
GLuint vertexshader[3], fragmentshader[3];		//---세이더 객체
GLuint vao[3];
GLuint vbo[3];
GLuint vbo_color[3];
GLuint ebo[3];

//---쉐이더 프로그램
GLuint s_program;
GLuint s_program_line;
GLuint s_program_floor;

//-----색
GLfloat cube_color[3][24];


void RandRGB()
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<GLclampf> rd_RGB(0.0, 1.0f);

	for (int i = 0; i < 24; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			cube_color[j][i] = rd_RGB(eng);
		}
	}
}

glm::vec3 Cube_trans[4] = { glm::vec3(0, 0, 0),
							glm::vec3(0, 0.5, 0) ,
							glm::vec3(-0.15, 0.3, 0) ,
							glm::vec3(0.15, 0.3, 0) };
glm::vec3 Cube_rotate[4] = { glm::vec3(0, 0, 0),
							glm::vec3(0, 0, 0) ,
							glm::vec3(0, 0, 0) ,
							glm::vec3(0, 0, 0) };
glm::vec3 pos_trans(0.0f, 3.0f, 5.0f);
glm::vec3 dir_trans(0.0f, 0.0f, 0.0f);
GLfloat rotate_screen = 0.0;
GLfloat rotate_camera = 0.0;

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


	RandRGB();
	InitShader();
	InitBuffer();


	glEnable(GL_DEPTH_TEST);


	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	//glutTimerFunc(10, TimerFunction, 0);

	glutMainLoop();
}
GLvoid drawScene()
{
	//---배경 초기화
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//---카메라 설정
	glm::vec4 cameraPos_trans(0.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 cameraDirection_trans(0.0f, -3.0f, -5.0f, 1.0f);

	glm::mat4 pos = glm::mat4(1.0f);
	pos = glm::rotate(pos, (GLfloat)glm::radians(rotate_screen), glm::vec3(0.0, 1.0, 0.0));
	pos = glm::translate(pos, glm::vec3(pos_trans.x, pos_trans.y, pos_trans.z));
	cameraPos_trans = pos * cameraPos_trans;

	glm::mat4 dir = glm::mat4(1.0f);
	dir = glm::translate(dir, glm::vec3(dir_trans.x, dir_trans.y, dir_trans.z));
	dir = glm::rotate(dir, (GLfloat)glm::radians(rotate_camera), glm::vec3(0.0, 1.0, 0.0));

	cameraDirection_trans = pos * dir * cameraDirection_trans;

	glm::vec3 cameraPos = glm::vec3(cameraPos_trans.x, cameraPos_trans.y, cameraPos_trans.z);		 //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(cameraDirection_trans.x, cameraDirection_trans.y, cameraDirection_trans.z); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);		 //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLoc_shape = glGetUniformLocation(s_program, "view"); //--- 뷰잉 변환 설정
	unsigned int viewLoc_line = glGetUniformLocation(s_program_line, "view"); //--- 뷰잉 변환 설정
	unsigned int viewLoc_floor = glGetUniformLocation(s_program_floor, "view"); //--- 뷰잉 변환 설정

	//---투영변환
	glm::mat4 projection = glm::mat4(1.0f);

	projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
	//projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- 공간을 약간 뒤로 미뤄줌


	unsigned int projLoc_shape = glGetUniformLocation(s_program, "projection");
	unsigned int projLoc_line = glGetUniformLocation(s_program_line, "projection");
	unsigned int projLoc_floor = glGetUniformLocation(s_program_floor, "projection");

	//----------- 선 그리기
	glUseProgram(s_program_line);
	glUniformMatrix4fv(viewLoc_line, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc_line, 1, GL_FALSE, &projection[0][0]);
	glPointSize(10.0);
	glDrawArrays(GL_LINES, 0, 6);

	//----------- 바닥 그리기
	glUseProgram(s_program_floor);
	glUniformMatrix4fv(viewLoc_floor, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc_floor, 1, GL_FALSE, &projection[0][0]);
	glPointSize(10.0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	//---------- 사각형 1 그리기
	glUseProgram(s_program);

	unsigned int modelLoc = glGetUniformLocation(s_program, "model");

	glUniformMatrix4fv(viewLoc_shape, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc_shape, 1, GL_FALSE, &projection[0][0]);

	glm::mat4 Model_transfrom[4] = { glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f),glm::mat4(1.0f) };
	for (int i = 0; i < 4; i++)
	{
		if(i == 3)
			glBindVertexArray(vao[2]);
		else
			glBindVertexArray(vao[i]);

		if(i > 0 && i < 3)
			Model_transfrom[i] *= Model_transfrom[i - 1];
		else if(i == 3)
			Model_transfrom[i] *= Model_transfrom[1];

		Model_transfrom[i] = glm::translate(Model_transfrom[i], glm::vec3(Cube_trans[i].x, Cube_trans[i].y, Cube_trans[i].z));
		Model_transfrom[i] = glm::rotate(Model_transfrom[i], (GLfloat)glm::radians(Cube_rotate[i].x), glm::vec3(1.0, 0.0, 0.0));
		Model_transfrom[i] = glm::rotate(Model_transfrom[i], (GLfloat)glm::radians(Cube_rotate[i].y), glm::vec3(0.0, 1.0, 0.0));
		Model_transfrom[i] = glm::rotate(Model_transfrom[i], (GLfloat)glm::radians(Cube_rotate[i].z), glm::vec3(0.0, 0.0, 1.0));
		Model_transfrom[i] = glm::scale(Model_transfrom[i], glm::vec3(1.0, 1.0, 1.0));

		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(Model_transfrom[i]));

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}

	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void make_vertexShader()
{
	vertexsource[0] = filetobuf("vertex_shape.glsl");
	vertexsource[1] = filetobuf("vertex_line.glsl");
	vertexsource[2] = filetobuf("vertex_floor.glsl");

	for (int i = 0; i < 3; i++)
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
	fragmentsource[0] = filetobuf("fragment_shape.glsl");
	fragmentsource[1] = filetobuf("fragment_line.glsl");
	fragmentsource[2] = filetobuf("fragment_floor.glsl");

	for (int i = 0; i < 3; i++)
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

void LinkShader(GLuint &program, GLuint &vertex, GLuint &fragment)
{
	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);

	// ---세이더가 잘 연결되었는지 체크하기
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(program, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}
}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	s_program_line = glCreateProgram();
	s_program_floor = glCreateProgram();


	LinkShader(s_program, vertexshader[0], fragmentshader[0]);
	LinkShader(s_program_line, vertexshader[1], fragmentshader[1]);
	LinkShader(s_program_floor, vertexshader[2], fragmentshader[2]);

	//--- 세이더 삭제하기
	for (int i = 0; i < 3; i++)
	{
		glDeleteShader(vertexshader[i]);
		glDeleteShader(fragmentshader[i]);
	}
}

void InitBuffer()
{
	glGenVertexArrays(3,vao);
	glGenBuffers(3, vbo);
	glGenBuffers(3, vbo_color);
	glGenBuffers(3, ebo);


	obj objfile[3];
	objfile[0].OpenFile("cube_18_1.obj");
	objfile[1].OpenFile("cube_18_2.obj");
	objfile[2].OpenFile("cube_18_3.obj");



	for (int i = 0; i < 3; i++)
	{
		objfile[i].ReadObj();

		glBindVertexArray(vao[i]);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
		glBufferData(GL_ARRAY_BUFFER, objfile[i].vertexNum * 3 * sizeof(GLfloat), objfile[i].vertex, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, objfile[i].faceNum * 3 * sizeof(int), objfile[i].face_v, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo_color[i]);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cube_color[i], GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
	}
}

int isMove_x = 0;
int isRotate = 0;
int isRotateArm = 0;
int isCameraRotate_ori = 0;


GLvoid TimerFunction(int value)
{

	if (value == 1 && isMove_x != 0)
	{
		Cube_trans[0].x += isMove_x * 0.01;

		glutTimerFunc(10, TimerFunction, 1);
	}
	if (value == 2 && isRotate != 0)
	{
		Cube_rotate[1].y += isRotate;

		glutTimerFunc(10, TimerFunction, 2);
	}
	if (value == 3 && isRotateArm != 0)
	{
		Cube_rotate[2].x += isRotateArm;
		Cube_rotate[3].x -= isRotateArm;

		if (Cube_rotate[2].x >= 90 || Cube_rotate[2].x <= -90)
			isRotateArm *= -1;
		glutTimerFunc(10, TimerFunction, 3);
	}
	//-----카메라 이동
	if (value == 4 && isCameraRotate_ori != 0)
	{
		rotate_screen += isCameraRotate_ori * 0.5;

		glutTimerFunc(10, TimerFunction, 4);
	}
	glutPostRedisplay();
}



GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'b':
		if (isMove_x == 0)
			glutTimerFunc(10, TimerFunction, 1);
		
		if (isMove_x == 1)
			isMove_x = 0;
		else
			isMove_x = 1;
		break;
	case 'B':
		if (isMove_x == 0)
			glutTimerFunc(10, TimerFunction, 1);

		if (isMove_x == -1)
			isMove_x = 0;
		else
			isMove_x = -1;
		break;
	case 'm':
		if (isRotate == 0)
			glutTimerFunc(10, TimerFunction, 2);

		if (isRotate == 1)
			isRotate = 0;
		else
			isRotate = 1;
		break;
	case 'M':
		if (isRotate == 0)
			glutTimerFunc(10, TimerFunction, 2);

		if (isRotate == -1)
			isRotate = 0;
		else
			isRotate = -1;
		break;
	case 't':
		if (isRotateArm == 0)
			glutTimerFunc(10, TimerFunction, 3);

		if (isRotateArm == 1)
			isRotateArm = 0;
		else
			isRotateArm = 1;
		break;
	case 'T':
		if (isRotateArm == 0)
			glutTimerFunc(10, TimerFunction, 3);

		if (isRotateArm == -1)
			isRotateArm = 0;
		else
			isRotateArm = -1;
		break;
	case 'z':
		pos_trans.z += 0.1;
		break;
	case 'Z':
		pos_trans.z -= 0.1;
		break;
	case 'x':
		pos_trans.x += 0.1;
		break;
	case 'X':
		pos_trans.x -= 0.1;
		break;
	case 'y':
		rotate_camera += 1.0;
		break;
	case 'Y':
		rotate_camera -= 1.0;
		break;
	case 'r':
		rotate_screen += 1.0;
		break;
	case 'R':
		rotate_screen -= 1.0;
		break;
	case 'a':
		if (isCameraRotate_ori == 0)
		{
			isCameraRotate_ori = 1;
			glutTimerFunc(10, TimerFunction, 4);
		}
		else if (isCameraRotate_ori == 1)
			isCameraRotate_ori = 0;
		break;
	case 'A':
		isCameraRotate_ori = 0;
		break;
	case 's':
		isMove_x = 0;
		isRotate = 0;
		isRotateArm = 0;
		isCameraRotate_ori = 0;
		break;
	case 'S':
		isMove_x = 0;
		isRotate = 0;
		isRotateArm = 0;
		isCameraRotate_ori = 0;
		break;
	case 'c':
		isMove_x = 0;
		isRotate = 0;
		isRotateArm = 0;
		isCameraRotate_ori = 0;

		Cube_trans[0] = glm::vec3(0, 0, 0);
		Cube_trans[1] = glm::vec3(0, 0.5, 0);
		Cube_trans[2] = glm::vec3(-0.15, 0.3, 0);
		Cube_trans[3] = glm::vec3(0.15, 0.3, 0);
		Cube_rotate[0] = glm::vec3(0, 0, 0);
		Cube_rotate[1] = glm::vec3(0, 0, 0);
		Cube_rotate[2] = glm::vec3(0, 0, 0);
		Cube_rotate[3] = glm::vec3(0, 0, 0);


		pos_trans = glm::vec3 (0.0f, 3.0f, 5.0f);
		dir_trans = glm::vec3 (0.0f, 0.0f, 0.0f);
		rotate_screen = 0.0;
		rotate_camera = 0.0;
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