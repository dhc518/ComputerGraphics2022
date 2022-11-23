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
#include "file.h"
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

//---glsl
void make_vertexShader();
void make_fragmentShader();
void InitShader();
void InitBuffer();
char* filetobuf(const char* file);
GLchar* vertexsource[2], * fragmentsource[2];		//---소스코드 저장 변수
GLuint vertexshader[2], fragmentshader[2];		//---세이더 객체
GLuint vao, vao_pyramid;
GLuint vbo[2], vbo_pyramid[3];
GLuint ebo, ebo_pyramid;
//---쉐이더 프로그램
GLuint s_program;
GLuint s_program_line;

//--육면체 색
GLfloat cubeColor[24];
GLfloat pyramidColor[15];

GLfloat HalfSizeOfCube = 0.5f;

//--애니메이션 재생
bool Y_rotating = false;
bool UpSideAnim = false;
int FrontSideAnim = 0;
int LeftRightSideAnim = 0;
bool DrawPyramid = false;
int OpenPyramid = 0;
bool ortho_anim = false;

//---자전 회전 
GLfloat y_radian = 0;
bool depthTest = true;

//---각 면 정보 클래스
class sidestate
{
private:
	glm::vec3 trans;
	glm::vec3 rotate;
	glm::vec3 pibot;
public:
	sidestate()
	{
		trans.x = 0.0; trans.y = 0.0; trans.z = 0.0;
		rotate.x = 0.0; rotate.y = 0.0; rotate.z = 0.0;
		pibot.x = 0.0; pibot.y = 0.0; pibot.z = 0.0;
	}
	//---멤버변수 SET
	void PlusTrans(GLfloat x, GLfloat y, GLfloat z)
	{
		trans.x += x; trans.y += y; trans.z += z;
	}
	void SetTrans(GLfloat x, GLfloat y, GLfloat z)
	{
		trans.x = x; trans.y = y; trans.z = z;
	}
	void PlusRotate(GLfloat x, GLfloat y, GLfloat z)
	{
		rotate.x += x; rotate.y += y; rotate.z += z;
	}
	void SetRotate(GLfloat x, GLfloat y, GLfloat z)
	{
		rotate.x = x; rotate.y = y; rotate.z = z;
	}
	void SetPibot(GLfloat x, GLfloat y, GLfloat z)
	{
		pibot.x = x; pibot.y = y; pibot.z = z;
	}

	//---멤버변수 GET
	glm::vec3 GetTrans() { return trans; }
	glm::vec3 GetRotate() { return rotate; }
	glm::vec3 GetPibot() { return pibot; }

	glm::mat4 RotateAtPibot(glm::mat4 TR)
	{
		TR = glm::translate(TR, glm::vec3(pibot.x, pibot.y, pibot.z));
		TR = glm::rotate(TR, (GLfloat)glm::radians(rotate.x), glm::vec3(1.0, 0.0, 0.0));
		TR = glm::rotate(TR, (GLfloat)glm::radians(rotate.y), glm::vec3(0.0, 1.0, 0.0));
		TR = glm::rotate(TR, (GLfloat)glm::radians(rotate.z), glm::vec3(0.0, 0.0, 1.0));
		TR = glm::translate(TR, glm::vec3(-1 * pibot.x, -1 * pibot.y, -1 * pibot.z));

		return TR;
	}
	glm::mat4 Translate(glm::mat4 TR)
	{
		TR = glm::translate(TR, glm::vec3(trans.x, trans.y, trans.z));

		return TR;
	}
};

sidestate cubeside[6];
sidestate pyramidside[4];


void RandRGB()
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<GLclampf> rd_RGB(0.0, 1.0f);

	for (int i = 0; i < 24; i++)
	{
		cubeColor[i] = rd_RGB(eng);
	}
	for (int i = 0; i < 15; i++)
	{
		pyramidColor[i] = rd_RGB(eng);
	}
}


int side = 0;
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

	//1,3 옆면, 2윗면, 5앞면
	cubeside[2].SetPibot(0, HalfSizeOfCube, 0);
	cubeside[5].SetPibot(0, -1 * HalfSizeOfCube, HalfSizeOfCube);

	pyramidside[0].SetPibot(0, 0, -0.4);
	pyramidside[1].SetPibot(-0.4, 0, 0);
	pyramidside[2].SetPibot(0, 0, 0.4);
	pyramidside[3].SetPibot(0.4, 0, 0);

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	glutMainLoop();
}
GLvoid drawScene()
{
	cout << "draw" << endl;
	//---배경 초기화
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//---카메라 설정
	glm::vec3 cameraPos = glm::vec3(-0.5f, 0.5f, 1.0f);		 //--- 카메라 위치
	glm::vec3 cameraDirection = glm::vec3(0.0f, 0.0f, 0.0f); //--- 카메라 바라보는 방향
	glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);		 //--- 카메라 위쪽 방향
	glm::mat4 view = glm::mat4(1.0f);

	view = glm::lookAt(cameraPos, cameraDirection, cameraUp);
	unsigned int viewLoc_shape = glGetUniformLocation(s_program, "view"); //--- 뷰잉 변환 설정
	unsigned int viewLoc_line = glGetUniformLocation(s_program_line, "view"); //--- 뷰잉 변환 설정

	//---투영변환
	glm::mat4 projection = glm::mat4(1.0f);
	if (ortho_anim == true)
		projection = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, -2.0f, 2.0f);
	else
	{
		projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 50.0f);
		projection = glm::translate(projection, glm::vec3(0.0, 0.0, -5.0)); //--- 공간을 약간 뒤로 미뤄줌
	}


	unsigned int projLoc_shape = glGetUniformLocation(s_program, "projection");
	unsigned int projLoc_line = glGetUniformLocation(s_program_line, "projection");

	//-------------선 그리기
	glUseProgram(s_program_line);
	glUniformMatrix4fv(viewLoc_line, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc_line, 1, GL_FALSE, &projection[0][0]);
	glPointSize(10.0);
	glDrawArrays(GL_LINES, 0, 6);
	glDrawArrays(GL_POINTS, 0, 6);



	//---------- 육면체 그리기(우)
	glUseProgram(s_program);
	glUniformMatrix4fv(viewLoc_shape, 1, GL_FALSE, &view[0][0]);
	glUniformMatrix4fv(projLoc_shape, 1, GL_FALSE, &projection[0][0]);

	unsigned int modelLoc = glGetUniformLocation(s_program, "model");

	glm::mat4 TR_cube = glm::mat4(1.0f);
	glm::mat4 T_cube = glm::mat4(1.0f);
	glm::mat4 Rx_cube = glm::mat4(1.0f);
	glm::mat4 Ry_cube = glm::mat4(1.0f);
	glm::mat4 S_cube = glm::mat4(1.0f);
	glm::mat4 OS_cube = glm::mat4(1.0f);

	T_cube = glm::translate(T_cube, glm::vec3(0, 0, 0));
	Rx_cube = glm::rotate(Rx_cube, (GLfloat)glm::radians(0.0), glm::vec3(1.0, 0.0, 0.0));
	Ry_cube = glm::rotate(Ry_cube, (GLfloat)glm::radians(y_radian), glm::vec3(0.0, 1.0, 0.0));
	S_cube = glm::scale(S_cube, glm::vec3(1.0, 1.0, 1.0));
	if (DrawPyramid == false)
	{
		glBindVertexArray(vao);
		for (int i = 0; i < 6; i++)
		{
			TR_cube = T_cube * Rx_cube * Ry_cube * S_cube;
			TR_cube = cubeside[i].Translate(TR_cube);
			TR_cube = cubeside[i].RotateAtPibot(TR_cube);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TR_cube));

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * i * 6));
		}
	}
	else
	{
		glBindVertexArray(vao_pyramid);
		TR_cube = T_cube * Rx_cube * Ry_cube * S_cube;
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TR_cube));
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0));
		for (int i = 2; i < 6; i++)
		{
			TR_cube = T_cube * Rx_cube * Ry_cube * S_cube;
			//TR_cube = cubeside[i].Translate(TR_cube);
			TR_cube = pyramidside[i - 2].RotateAtPibot(TR_cube);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(TR_cube));
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * i * 3));
		}

	}

	glBindVertexArray(0);

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
	fragmentsource[0] = filetobuf("fragment_shape.glsl");
	fragmentsource[1] = filetobuf("fragment_line.glsl");

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
	//----------------육면체

	obj objfile("cube_16.obj");
	objfile.ReadObj();
	glGenVertexArrays(1, &vao);
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

	//----------------오면체
	obj objfile_pyramid("pyramid_16.obj");
	objfile_pyramid.ReadObj();
	glGenVertexArrays(1, &vao_pyramid);
	glGenBuffers(2, vbo_pyramid);
	glGenBuffers(1, &ebo_pyramid);
	glBindVertexArray(vao_pyramid);




	glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[0]);
	glBufferData(GL_ARRAY_BUFFER, objfile_pyramid.vertexNum * 3 * sizeof(GLfloat), objfile_pyramid.vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_pyramid);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, objfile_pyramid.faceNum * 3 * sizeof(int), objfile_pyramid.face_v, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_pyramid[1]);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), pyramidColor, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);

	glEnable(GL_DEPTH_TEST);

}


bool FrontSideAinm_Open = true;
GLfloat LeftRightSideAnimTrans = 0;

GLvoid TimerFunction(int value)
{
	if (value == 0 && Y_rotating == true)
	{
		y_radian += 1.0f;
		glutTimerFunc(10, TimerFunction, 0);
	}
	else if (value == 1 && UpSideAnim == true)
	{
		cubeside[2].PlusRotate(1.0f, 0.0, 0.0);
		glutTimerFunc(10, TimerFunction, 1);
	}
	else if (value == 2 && FrontSideAnim != 0)
	{
		cubeside[5].PlusRotate((GLfloat)FrontSideAnim * 1.0, 0.0, 0.0);
		if (cubeside[5].GetRotate().x >= 90.0f)
		{
			cubeside[5].SetRotate(90.0, 0.0, 0.0);
			FrontSideAnim = 0;
		}
		else if (cubeside[5].GetRotate().x <= 0)
		{
			cubeside[5].SetRotate(0.0, 0.0, 0.0);
			FrontSideAnim = 0;
		}



		glutTimerFunc(10, TimerFunction, 2);
	}
	else if (value == 3 && LeftRightSideAnim != 0)
	{
		cubeside[1].PlusTrans(0.0, (GLfloat)LeftRightSideAnim * 0.01, 0.0);
		cubeside[3].PlusTrans(0.0, (GLfloat)LeftRightSideAnim * 0.01, 0.0);

		if (cubeside[1].GetTrans().y >= 1.0f)
		{
			cubeside[1].SetTrans(0.0, 1.0, 0.0);
			cubeside[3].SetTrans(0.0, 1.0, 0.0);

			LeftRightSideAnim = 0;
		}
		else if (cubeside[1].GetTrans().y <= 0.0f)
		{
			cubeside[1].SetTrans(0.0, 0.0, 0.0);
			cubeside[3].SetTrans(0.0, 0.0, 0.0);

			LeftRightSideAnim = 0;
		}

		glutTimerFunc(10, TimerFunction, 3);
	}
	else if (value == 4 && OpenPyramid != 0)
	{
		pyramidside[0].PlusRotate((GLfloat)OpenPyramid * -1, 0.0, 0.0);
		pyramidside[1].PlusRotate(0.0, 0.0, (GLfloat)OpenPyramid);
		pyramidside[2].PlusRotate((GLfloat)OpenPyramid, 0.0, 0.0);
		pyramidside[3].PlusRotate(0.0, 0.0, (GLfloat)OpenPyramid * -1);

		if (pyramidside[2].GetRotate().x >= 248.0f)
		{
			pyramidside[0].SetRotate(-248.0f, 0.0, 0.0);
			pyramidside[1].SetRotate(0.0, 0.0, 248.0f);
			pyramidside[2].SetRotate(248.0f, 0.0, 0.0);
			pyramidside[3].SetRotate(0.0, 0.0, -248.0f);
			OpenPyramid = 0;
		}
		else if (pyramidside[2].GetRotate().x <= 0.0f)
		{
			pyramidside[0].SetRotate(0.0, 0.0, 0.0);
			pyramidside[1].SetRotate(0.0, 0.0, 0.0);
			pyramidside[2].SetRotate(0.0, 0.0, 0.0);
			pyramidside[3].SetRotate(0.0, 0.0, 0.0);

			OpenPyramid = 0;
		}
		glutTimerFunc(10, TimerFunction, 4);
	}
	glutPostRedisplay();
}



GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'h':
		if (depthTest == true)
		{
			depthTest = false;
			glDisable(GL_DEPTH_TEST);
		}
		else
		{
			depthTest = true;
			glEnable(GL_DEPTH_TEST);
		}
		break;
	case 'y':
		if (Y_rotating == false)
		{
			Y_rotating = true;
			glutTimerFunc(10, TimerFunction, 0);
		}
		else
		{
			Y_rotating = false;
			//y_radian = 0;
		}
		break;
	case 't':
		DrawPyramid = false;
		if (UpSideAnim == false)
		{
			UpSideAnim = true;
			glutTimerFunc(10, TimerFunction, 1);
		}
		break;
	case 'T':
		DrawPyramid = false;
		UpSideAnim = false;
		break;
	case 'f':
		DrawPyramid = false;
		if (FrontSideAnim == 0)
			glutTimerFunc(10, TimerFunction, 2);
		FrontSideAnim = 1;
		break;
	case 'F':
		DrawPyramid = false;
		if (FrontSideAnim == 0)
			glutTimerFunc(10, TimerFunction, 2);
		FrontSideAnim = -1;
		break;
	case '1':
		DrawPyramid = false;
		if (LeftRightSideAnim == 0)
			glutTimerFunc(10, TimerFunction, 3);
		LeftRightSideAnim = 1;
		break;
	case '2':
		DrawPyramid = false;
		if (LeftRightSideAnim == 0)
			glutTimerFunc(10, TimerFunction, 3);
		LeftRightSideAnim = -1;
		break;
	case 'o':
		DrawPyramid = true;
		if (OpenPyramid == 0)
			glutTimerFunc(10, TimerFunction, 4);
		OpenPyramid = 1;
		break;
	case 'O':
		DrawPyramid = true;
		if (OpenPyramid == 0)
			glutTimerFunc(10, TimerFunction, 4);
		OpenPyramid = -1;
		break;
	case 'p':
		ortho_anim = true;
		break;
	case 'P':
		ortho_anim = false;
		break;
	case 'q':
		glutDestroyWindow(windowID);
		break;
	}

	glutPostRedisplay();
}