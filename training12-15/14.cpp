#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "file.h"
#include <random>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>
//#include <glm/glm/glm.hpp>
//#include <glm/glm/ext.hpp>
//#include <glm/glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

#define WIDTH 800
#define HEIGHT 600

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0, 1);

typedef struct FACE {
	int a, b, c;
};

typedef struct READ {
	int vertexNum;
	int faceNum;
	vec3* vertex;
	FACE* face;
};

//변수
GLchar* vertexsource[2], * fragmentsource[2];
GLuint vertexshader[2], fragmentshader[2], vao[2], vbo[2], ebo[1];
GLuint s_program, s_program_plat, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLUquadricObj* qobj;

GLfloat middle_line[6][3] = {};

READ cube;
READ tetra;

bool c_select = true;

float x_rotation1 = 0;
float y_rotation1 = 0;
float x_rotation2 = 0;
float y_rotation2 = 0;
float y_rotation3 = 0;


bool r_plus = false;
bool r_minus = false;
bool x_plus1 = false;
bool y_plus1 = false;
bool x_minus1 = false;
bool y_minus1 = false;
bool x_plus2 = false;
bool y_plus2 = false;
bool x_minus2 = false;
bool y_minus2 = false;

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShader();
void make_fragmentShader();
void InitBuffer();
void InitShader();

void ReadObj(FILE* objFile, READ& Read);

//그리기 함수
void draw_middle_line();

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);


void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WIDTH, HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized\n";

	draw_middle_line();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	int vColorLocation2 = glGetUniformLocation(s_program, "out_Color");
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);

	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(0);

	mat4 xyz = mat4(1.0f);
	xyz = rotate(xyz, radians(30.f), vec3(1.f, 0, 0));
	xyz = rotate(xyz, radians(30.f), vec3(0, -1.f, 0));

	int xyzLocation = glGetUniformLocation(s_program, "modelTransform");
	//--- modelTransform 변수에 변환 값 적용하기
	glUniformMatrix4fv(xyzLocation, 1, GL_FALSE, value_ptr(xyz));

	//중앙선
	glUniform3f(vColorLocation2, 0, 0, 0);
	glDrawArrays(GL_LINES, 0, 6);


	//도형 그리기
	glEnable(GL_DEPTH_TEST);

	//glUseProgram(s_program);
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");

	mat4 model = mat4(1.0f);  mat4 cube_model = mat4(1.f);
	mat4 RT = mat4(1.f); mat4 RT1 = mat4(1.f); mat4 RT2 = mat4(1.f); mat4 RT3 = mat4(1.f); mat4 RT4 = mat4(1.f);

	model = rotate(model, radians(30.f), vec3(1.f, 0, 0));
	model = rotate(model, radians(30.f), vec3(0, -1.f, 0));
	model = translate(model, vec3(0.5f, 0, 0));
	RT = rotate(RT, radians(x_rotation1), vec3(1.f, 0, 0));
	RT1 = rotate(RT1, radians(y_rotation1), vec3(0, 1.f, 0));
	RT2 = rotate(RT2, radians(30.f), vec3(1.f, 0, 0));
	RT2 = rotate(RT2, radians(y_rotation3), vec3(0, 1.f, 0));

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	//--- modelTransform 변수에 변환 값 적용하기
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RT2_1 * TR * RT * RT1 * model));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RT2 * model * RT * RT1));


	glUniform3f(vColorLocation, 0, 0, 0);

	if (c_select) {
		qobj = gluNewQuadric();
		gluQuadricDrawStyle(qobj, GLU_LINE);// 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); //생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE);// 생략 가능
		gluSphere(qobj, 0.3, 50, 50);
	}

	else if (!c_select) {
		qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); // 생략 가능
		gluCylinder(qobj, 0.3, 0.0, 0.5, 20, 8);
	}

	
	//큐브
	cube_model = rotate(cube_model, radians(30.f), vec3(1.f, 0, 0));
	cube_model = rotate(cube_model, radians(30.f), vec3(0, -1.f, 0));
	cube_model = translate(cube_model, vec3(-0.5f, 0, 0));
	RT3 = rotate(RT3, radians(x_rotation2), vec3(1.f, 0, 0));
	RT4 = rotate(RT4, radians(y_rotation2), vec3(0, 1.f, 0));

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RT2 * cube_model * RT3 * RT4));


	if (c_select) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		glBindVertexArray(vao[1]);
		glEnableVertexAttribArray(0);

		for (int i = 0; i < 6; i++)
		{

			if (i == 0)
				glUniform3f(vColorLocation, 1.f, 0, 0);

			if (i == 2)
				glUniform3f(vColorLocation, 0, 1.f, 0);

			if (i == 4)
				glUniform3f(vColorLocation, 0, 0, 1.f);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * i * 6));

		}
	}

	if (!c_select) {
		qobj = gluNewQuadric(); // 객체 생성하기
		gluQuadricDrawStyle(qobj, GLU_LINE); // 도형 스타일
		gluQuadricNormals(qobj, GLU_SMOOTH); // 생략 가능
		gluQuadricOrientation(qobj, GLU_OUTSIDE); // 생략 가능
		gluCylinder(qobj, 0.3, 0.3, 0.5, 20, 8);
	}

	glutSwapBuffers(); //--- 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
{
	glViewport(0, 0, w, h);
}

void make_vertexShader()
{
	vertexsource[0] = filetobuf("vertex.glsl");
	vertexsource[1] = filetobuf("vertex_2d.glsl");

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
	for (int i = 0; i < 2; i++)
	{
		fragmentsource[i] = filetobuf("fragment.glsl");

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

void InitBuffer()
{
	glGenVertexArrays(2, vao); //--- VAO 를 지정하고 할당하기
	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	glGenBuffers(1, ebo);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), middle_line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//정육면체
	FILE* fp;
	fp = fopen("cube.obj", "rb");
	ReadObj(fp, cube);

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vec3), cube.vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(FACE), cube.face, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();
	s_program_plat = glCreateProgram();

	glAttachShader(s_program, vertexshader[0]);
	glAttachShader(s_program, fragmentshader[0]);

	glAttachShader(s_program_plat, vertexshader[1]);
	glAttachShader(s_program_plat, fragmentshader[1]);

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

	glLinkProgram(s_program_plat);

	// ---세이더가 잘 연결되었는지 체크하기
	glGetProgramiv(s_program_plat, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(s_program_plat, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		return;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'r':
		r_plus = !r_plus;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'R':
		r_minus = !r_minus;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'a':
		x_plus1 = !x_plus1;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'A':
		x_minus1 = !x_minus1;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'b':
		y_plus1 = !y_plus1;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'B':
		y_minus1 = !y_minus1;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'q':
		glutLeaveMainLoop();
		break;

	case 'x':
		x_plus2 = !x_plus2;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'X':
		x_minus2 = !x_minus2;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'y':
		y_plus2 = !y_plus2;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'Y':
		y_minus2 = !y_minus2;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'c':
		c_select = !c_select;
		break;

	case 's':
		c_select = true;
		x_plus1 = false;
		y_plus1 = false;
		x_minus1 = false;
		y_minus1 = false;
		x_plus2 = false;
		y_plus2 = false;
		x_minus2 = false;
		y_minus2 = false;
		r_plus = false;
		r_minus = false;
		x_rotation1 = y_rotation1 = x_rotation2 = y_rotation2 = y_rotation3 = 0;
		break;
	}

	//glBindVertexArray(vao[1]);
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	//glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vec3), cube.vertex, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(FACE), cube.face, GL_STATIC_DRAW);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	//glEnableVertexAttribArray(0);

	glutPostRedisplay();
}

void Timer(int value)
{
	if (x_plus1) {
		x_rotation1 += 1;
	}

	if (y_plus1) {
		y_rotation1 += 1;
	}

	if (x_minus1) {
		x_rotation1 -= 1;
	}

	if (y_minus1) {
		y_rotation1 -= 1;
	}

	if (x_plus2) {
		x_rotation2 += 1;
	}

	if (y_plus2) {
		y_rotation2 += 1;
	}

	if (x_minus2) {
		x_rotation2 -= 1;
	}

	if (y_minus2) {
		y_rotation2 -= 1;
	}

	if (r_plus)
		y_rotation3 += 1;

	if (r_minus)
		y_rotation3 -= 1;

	if (!x_plus1 && !x_minus1 && !y_plus1 && !y_minus1 && !x_plus2 && !x_minus2 && !y_plus2 && !y_minus2 && !r_minus && !r_plus)
		return;

	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vec3), cube.vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(FACE), cube.face, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glutTimerFunc(10, Timer, 1);

	glutPostRedisplay();
}

void draw_middle_line()
{

	middle_line[0][0] = 1.f;  middle_line[0][1] = 0;  middle_line[0][2] = 0;
	middle_line[1][0] = -1.f;  middle_line[1][1] = 0;  middle_line[1][2] = 0;
	middle_line[2][0] = 0;  middle_line[2][1] = 1.f;  middle_line[2][2] = 0;
	middle_line[3][0] = 0;  middle_line[3][1] = -1.f;  middle_line[3][2] = 0;
	middle_line[4][0] = 0;  middle_line[4][1] = 0;  middle_line[4][2] = 1.f;
	middle_line[5][0] = 0;  middle_line[5][1] = 0;  middle_line[5][2] = -1.f;

}

void ReadObj(FILE* objFile, READ& Read)
{
	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	char count[100];
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			Read.vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			Read.faceNum += 1;
		memset(count, '\0', sizeof(count)); // 배열 초기화
	}

	//--- 2. 메모리 할당
	int vertIndex = 0;
	int faceIndex = 0;
	rewind(objFile);
	Read.vertex = (vec3*)malloc(sizeof(vec3) * Read.vertexNum);
	Read.face = (FACE*)malloc(sizeof(FACE) * Read.faceNum);
	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&Read.vertex[vertIndex].x, &Read.vertex[vertIndex].y,
				&Read.vertex[vertIndex].z);
			vertIndex++;
		} else if (count[0] == 'f' && count[1] == '\0') {
			fscanf(objFile, "%d %d %d",
				&Read.face[faceIndex].a, &Read.face[faceIndex].b, &Read.face[faceIndex].c);
			Read.face[faceIndex].a--; Read.face[faceIndex].b--; Read.face[faceIndex].c--;
			faceIndex++;
		}
	}
}