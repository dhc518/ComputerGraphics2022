#define Pro 6
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "file.h"
#include <random>

#define WIDTH 800
#define HEIGHT 600

#define PI 3.1415926535897932384626433832795

#if Pro == 6

//변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao, vbo;
GLuint s_program, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat size_x = 0.1f;
GLfloat size_y = 0.15f;

GLfloat center_verx, center_very; //삼각형 중심 좌표
GLfloat interval_x = 0.2f, interval_y = 0.3f; //점 사이의 거리
int tri_select = 0;

GLfloat triShape[4][3][2] = { //--- 삼각형 위치 값
0};

GLfloat colors[4][3] = {};


//함수
GLvoid drawScene(GLvoid);
int leftmouse_count = 0;//새로그린 도형 제일 위로 올리기 위한 마우스 카운터
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();
void draw_triangle();

void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);


void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
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


	draw_triangle();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);

	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);

	//--- 삼각형 그리기
	int i = 0;
	while (i < 4) {
		int triangle_num = (leftmouse_count % 4 + i) % 4;//최근 삼각형 위로 불러오기
		glUniform3f(vColorLocation, colors[triangle_num][0], colors[triangle_num][1], colors[triangle_num][2]);
		glDrawArrays(GL_TRIANGLES, triangle_num * 3, 3);
		i++;
	}

	glutSwapBuffers(); //--- 화면에 출력하기

}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
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
	if(!result)
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

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기

	glBindVertexArray(vao); //--- VAO를 바인드하기

	glGenBuffers(1, &vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//--- 변수 diamond 에서 버텍스 데이터 값을 버퍼에 복사한다.
	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	////--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	////--- 변수 colors에서 버텍스 색상을 복사한다.
	////--- colors 배열의 사이즈: 9 *float 
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	////--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	////--- attribute 인덱스 1번을 사용 가능하게 함.
	//glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기

	make_fragmentShader(); //--- 프래그먼트 세이더 만들기

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");
	// 
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDTH / 2)) / (WIDTH / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		leftmouse_count++;//왼쪽 마우스 클릭 카운터
		interval_x += size_x;
		interval_y += size_y;
		triShape[tri_select][0][0] = real_x + interval_x ; triShape[tri_select][0][1] = real_y - interval_y;   //오른쪽 점
		triShape[tri_select][1][0] = real_x - interval_x; triShape[tri_select][1][1] = real_y - interval_y;   //왼쪽 점
		triShape[tri_select][2][0] = real_x; triShape[tri_select][2][1] = real_y + interval_y;	      //위쪽 점
		if (interval_x > (GLfloat)0.4f || interval_x < 0.2f) {
			size_x *= -1;
			size_y *= -1;
		}

		tri_select++;
		tri_select %= 4;
		std::cout << interval_x << std::endl;
		std::cout << tri_select << std::endl;
		std::cout << real_x << " " << real_y << std::endl;
	}
	InitBuffer();
	glutPostRedisplay();
}

void draw_triangle()
{
	center_verx = -0.5f;
	center_very = 0.5f;
	for (int i = 0; i < 4; i++) {
		if (i == 2)
			center_very *= -1;

		triShape[i][0][0] = center_verx + 0.2f; triShape[i][0][1] = center_very - 0.3f;   //오른쪽 점
		triShape[i][1][0] = center_verx - 0.2f; triShape[i][1][1] = center_very - 0.3f;   //왼쪽 점
		triShape[i][2][0] = center_verx; triShape[i][2][1] = center_very + 0.3f;	      //위쪽 점

		std::cout << triShape[i][0][0] << " " << triShape[i][0][1] << std::endl;
		std::cout << triShape[i][1][0] << " " << triShape[i][1][1] << std::endl;
		std::cout << triShape[i][2][0] << " " << triShape[i][2][1] << std::endl;
		std::cout << "\n" << std::endl;

		center_verx *= -1;
		if(i < 3 )
			colors[i][i] = 1.f;
	}
	colors[3][0] = 0.5f; colors[3][1] = 0.5f;
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		std::cout << "a" << std::endl;
		break;

	case 'b':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		std::cout << "b" << std::endl;
		break;
	}
	glutPostRedisplay();
}

#elif Pro == 7
//구조체
typedef struct RECTANGLE {
	GLfloat x1, x2, y1, y2;
	int shpaetype;
};

RECTANGLE Rectan[4];

//변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao, vbo;
GLuint s_program, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat center_verx[4] = { -0.5f, 0.5f ,-0.5f ,0.5f }, center_very[4] = { 0.2f, 0.2f, -0.8f, -0.8f }; //삼각형 중심 좌표
GLfloat interval_x = 0.2f, interval_y = 0.3f; //점 사이의 거리

GLfloat move_x[4] = { 0.03f, -0.01f, 0.01f, -0.02f };
GLfloat move_y[4] = { 0.01f, 0.02f, -0.005f, -0.01f };

GLfloat triShape[4][3][2] = {}; //--- 삼각형 위치 값
GLfloat colors[4][3] = {};

bool start = false;
float theta[4] = {0, 0, 0, 0};

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();
void draw_triangle();

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);


void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
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

	for (int i = 0; i < 4; i++) {
		Rectan[i].shpaetype = 3;
	}

	draw_triangle();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);

	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);

	//--- 삼각형 그리기
	for (int i = 0; i < 4; i++) {
		glUniform3f(vColorLocation, colors[i][0], colors[i][1], colors[i][2]);
		glDrawArrays(GL_TRIANGLES, i * 3, 3);
	}

	glutSwapBuffers(); //--- 화면에 출력하기

}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
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

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기

	glBindVertexArray(vao); //--- VAO를 바인드하기

	glGenBuffers(1, &vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	////--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	//glBindBuffer(GL_ARRAY_BUFFER, vbo);

	////--- 변수 colors에서 버텍스 색상을 복사한다.
	////--- colors 배열의 사이즈: 9 *float 
	//glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), colors, GL_STATIC_DRAW);

	////--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	////--- attribute 인덱스 1번을 사용 가능하게 함.
	//glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기

	make_fragmentShader(); //--- 프래그먼트 세이더 만들기

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");
	// 
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void draw_triangle()
{
	for (int i = 0; i < 4; i++) {

		triShape[i][0][0] = center_verx[i] + (0.1f * cos(theta[i])); triShape[i][0][1] = center_very[i] + (0.1f * sin(theta[i]));   //오른쪽 점
		triShape[i][1][0] = center_verx[i] - (0.1f * cos(theta[i])); triShape[i][1][1] = center_very[i] - (0.1f * sin(theta[i]));   //왼쪽 점
		triShape[i][2][0] = center_verx[i] - (0.6f * sin(theta[i])); triShape[i][2][1] = center_very[i] + (0.6f * cos(theta[i]));	      //위쪽 점

		Rectan[i].x1 = center_verx[i] - 0.1f * cos(theta[i]); Rectan[i].y1 = center_very[i] - 0.1f * sin(theta[i]);
		Rectan[i].x2 = center_verx[i] + (0.1f * cos(theta[i]) - 0.6f * sin(theta[i])); Rectan[i].y2 = center_very[i] + (0.1f * sin(theta[i]) + 0.6f * cos(theta[i]));

		if (i < 3)
			colors[i][i] = 1.f;
	}
	std::cout << "\n" << std::endl;
	std::cout << Rectan[0].x2 << ". " << Rectan[0].y2 << std::endl;
	std::cout << Rectan[0].x1 << " " << Rectan[0].y1 << std::endl;
	//std::cout << move_x[0] << std::endl;
	colors[3][0] = 0.5f; colors[3][1] = 0.5f;
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		start = !start;

		glutTimerFunc(10, Timer, 1);
		break;
	}
	glutPostRedisplay();
}


void Timer(int value)
{
	if (start == true) {
		for (int i = 0; i < 4; i++) {
			
			if (Rectan[i].x1 < -1.f || Rectan[i].x2 < -1.f) {
				move_x[i] *= -1;
				theta[i] = 270 * PI / 180;
				if (Rectan[i].shpaetype == 0) {
					center_verx[i] -= 0.6f;
				}
				Rectan[i].shpaetype = 2;
			}

			if (Rectan[i].x2 > 1.f || Rectan[i].x1 > 1.f) {
				move_x[i] *= -1;
				theta[i] = 90 * PI / 180;
				if (Rectan[i].shpaetype == 2) {
					center_verx[i] += 0.6f;
				}
				Rectan[i].shpaetype = 0;
			}

			if (Rectan[i].y1 < -1.f || Rectan[i].y2 < -1.f) {
				move_y[i] *= -1;
				theta[i] = 0;
				if (Rectan[i].shpaetype == 1) {
					center_very[i] -= 0.6f;
				}
				Rectan[i].shpaetype = 3;
			}

			if (Rectan[i].y2 > 1.f || Rectan[i].y1 > 1.f) {
				move_y[i] *= -1;
				theta[i] = 180 * PI / 180;
				if (Rectan[i].shpaetype == 3) {
					center_very[i] += 0.6f;
				}
				Rectan[i].shpaetype = 1;
			}
			center_verx[i] += move_x[i]; center_very[i] += move_y[i];
		}
	}
	draw_triangle();
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	glutPostRedisplay(); // 화면 재 출력
	if (!start)
		return;
	glutTimerFunc(100, Timer, 1);
}

#elif Pro == 8
//구조체
typedef struct RECTANGLE {
	GLfloat x1, x2, y1, y2;
	int shpaetype;
	int crash;
};

RECTANGLE Rectan[4];

//변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao[2], vbo[2], ebo;
GLuint s_program, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat center_verx[4] = { -0.5f, 0.5f ,-0.5f ,0.5f }, center_very[4] = { 0.2f, 0.2f, -0.8f, -0.8f }; //삼각형 중심 좌표
GLfloat interval_x = 0.2f, interval_y = 0.3f; //점 사이의 거리

GLfloat move_x[4] = { 0.05f, -0.05f, 0.07f, -0.04f };
GLfloat move_y[4] = { 0.07f, 0.06f, -0.05f, -0.05f };

GLfloat triShape[4][3][2] = {}; //--- 삼각형 위치 값
GLfloat colors[4][3] = {};
GLfloat Linevertex[4][2] = { {-0.3f, 0.3f}, {0.3f, 0.3f}, {-0.3f, -0.3f}, {0.3f, -0.3f}};
unsigned int rectshape[8] = {0, 1, 0, 2, 1, 3, 2, 3};

bool start = false;
float theta[4] = { 0, 0, 0, 0 };

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();
void draw_triangle();

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);


void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
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

	for (int i = 0; i < 4; i++) {
		Rectan[i].shpaetype = 3;
		Rectan[i].crash = 0;
	}

	draw_triangle();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);

	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(0);


	//--- 삼각형 그리기
	for (int i = 0; i < 4; i++) {
		glUniform3f(vColorLocation, colors[i][0], colors[i][1], colors[i][2]);
		glDrawArrays(GL_TRIANGLES, i * 3, 3);
	}

	glBindVertexArray(vao[1]);
	glEnableVertexAttribArray(0);

	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	glutSwapBuffers(); //--- 화면에 출력하기

}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
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

void InitBuffer()
{
	glGenVertexArrays(2, vao); //--- VAO 를 지정하고 할당하기

	glBindVertexArray(vao[0]); //--- VAO를 바인드하기

	glGenBuffers(2, vbo); //--- 2개의 VBO를 지정하고 할당하기
	glGenBuffers(1, &ebo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glBindVertexArray(vao[1]);

	////--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	////--- 변수 colors에서 버텍스 색상을 복사한다.
	////--- colors 배열의 사이즈: 9 *float 
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), Linevertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

	////--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * sizeof(unsigned int), rectshape, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

	////--- attribute 인덱스 1번을 사용 가능하게 함.
	glEnableVertexAttribArray(0);

}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기

	make_fragmentShader(); //--- 프래그먼트 세이더 만들기

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");
	// 
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void draw_triangle()
{
	for (int i = 0; i < 4; i++) {

		triShape[i][0][0] = center_verx[i] + (0.05f * cos(theta[i])); triShape[i][0][1] = center_very[i] + (0.05f * sin(theta[i]));   //오른쪽 점
		triShape[i][1][0] = center_verx[i] - (0.05f * cos(theta[i])); triShape[i][1][1] = center_very[i] - (0.05f * sin(theta[i]));   //왼쪽 점
		triShape[i][2][0] = center_verx[i] - (0.4f * sin(theta[i])); triShape[i][2][1] = center_very[i] + (0.4f * cos(theta[i]));	      //위쪽 점

		Rectan[i].x1 = center_verx[i] - 0.05f * cos(theta[i]); Rectan[i].y1 = center_very[i] - 0.05f * sin(theta[i]);
		Rectan[i].x2 = center_verx[i] + (0.05f * cos(theta[i]) - 0.4f * sin(theta[i])); Rectan[i].y2 = center_very[i] + (0.05f * sin(theta[i]) + 0.4f * cos(theta[i]));

		if (i < 3)
			colors[i][i] = 1.f;
	}
	std::cout << "\n" << std::endl;
	std::cout << Rectan[0].x2 << ". " << Rectan[0].y2 << std::endl;
	std::cout << Rectan[0].x1 << " " << Rectan[0].y1 << std::endl;
	//std::cout << move_x[0] << std::endl;
	colors[3][0] = 0.5f; colors[3][1] = 0.5f;
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		start = !start;

		glutTimerFunc(10, Timer, 1);
		break;
	}
	glutPostRedisplay();
}


void Timer(int value)
{
	if (start == true) {
		for (int i = 0; i < 4; i++) {

			if (Rectan[i].x1 < -1.f || Rectan[i].x2 < -1.f) {
				move_x[i] *= -1;
				theta[i] = 270 * PI / 180;
				if (Rectan[i].shpaetype == 0) {
					center_verx[i] -= 0.4f;
				}
				Rectan[i].shpaetype = 2;
				Rectan[i].crash = 2;
			}

			if (Rectan[i].x2 > 1.f || Rectan[i].x1 > 1.f) {
				move_x[i] *= -1;
				theta[i] = 90 * PI / 180;
				if (Rectan[i].shpaetype == 2) {
					center_verx[i] += 0.4f;
				}
				Rectan[i].shpaetype = 0;
				Rectan[i].crash = 2;
			}

			if (Rectan[i].y1 < -1.f || Rectan[i].y2 < -1.f) {
				move_y[i] *= -1;
				theta[i] = 0;
				if (Rectan[i].shpaetype == 1) {
					center_very[i] -= 0.4f;
				}
				Rectan[i].shpaetype = 3;
				Rectan[i].crash = 1;
			}

			if (Rectan[i].y2 > 1.f || Rectan[i].y1 > 1.f) {
				move_y[i] *= -1;
				theta[i] = 180 * PI / 180;
				if (Rectan[i].shpaetype == 3) {
					center_very[i] += 0.4f;
				}
				Rectan[i].shpaetype = 1;
				Rectan[i].crash = 1;
			}

			if (Rectan[i].x2 > Linevertex[0][0] && Rectan[i].x2 < Linevertex[1][0] && (Rectan[i].y2 < Linevertex[0][1] && Rectan[i].y2 > Linevertex[2][1]) ||
				(Rectan[i].x1 > Linevertex[0][0] && Rectan[i].x1 < Linevertex[1][0] && (Rectan[i].y1 < Linevertex[0][1] && Rectan[i].y1 > Linevertex[2][1]))) {
				if (Rectan[i].shpaetype == 2) {
					move_x[i] *= -1;
					theta[i] = 90 * PI / 180;
					if (Rectan[i].shpaetype == 2) {
						center_verx[i] += 0.4f;
					}
					Rectan[i].shpaetype = 0;
				}

				else if (Rectan[i].shpaetype == 0) {
					move_x[i] *= -1;
					theta[i] = 270 * PI / 180;
					if (Rectan[i].shpaetype == 0) {
						center_verx[i] -= 0.4f;
					}
					Rectan[i].shpaetype = 2;
				}

				else if (Rectan[i].shpaetype == 1) {

					move_y[i] *= -1;
					theta[i] = 0;
					if (Rectan[i].shpaetype == 1) {
						center_very[i] -= 0.4f;
					}
					Rectan[i].shpaetype = 3;
					
				}

				else if (Rectan[i].shpaetype == 3) {
					move_y[i] *= -1;
					theta[i] = 180 * PI / 180;
					if (Rectan[i].shpaetype == 3) {
						center_very[i] += 0.4f;
					}
					Rectan[i].shpaetype = 1;
				}
			}

			std::cout << Linevertex[0][0]<< std::endl;



			center_verx[i] += move_x[i]; center_very[i] += move_y[i];
		}
	}
	draw_triangle();
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), triShape, GL_STATIC_DRAW);
	glutPostRedisplay(); // 화면 재 출력
	if (!start)
		return;
	glutTimerFunc(100, Timer, 1);
}

#elif Pro == 9

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0, 1);

//변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao, vbo;
GLuint s_program, shaderID;

GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat Circleshape[300][2] = {};
GLfloat radius = 0;

bool start = true;
int pointcount = 0;
float theta = 0;
int radian = 0;
int dir = 1;

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();

void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Timer(int value);


void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
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

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	//--- 변경된 배경색 설정
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- 렌더링 파이프라인에 세이더 불러오기
	glUseProgram(s_program);

	//--- 사용할 VAO 불러오기
	glBindVertexArray(vao);

	glEnableVertexAttribArray(0);

	//--- 삼각형 그리기
	glPointSize((5.f));
	glDrawArrays(GL_POINTS, 0, pointcount);
	std::cout << pointcount << std::endl;

	glutSwapBuffers(); //--- 화면에 출력하기

}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
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

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO 를 지정하고 할당하기

	glBindVertexArray(vao); //--- VAO를 바인드하기

	glGenBuffers(1, &vbo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 600 * sizeof(GLfloat), Circleshape, GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glEnableVertexAttribArray(0);

	////--- 2번째 VBO를 활성화 하여 바인드 하고, 버텍스 속성 (색상)을 저장
	//glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);

	////--- 변수 colors에서 버텍스 색상을 복사한다.
	////--- colors 배열의 사이즈: 9 *float 
	//glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), Linevertex, GL_STATIC_DRAW);

	////--- 색상값을 attribute 인덱스 1번에 명시한다: 버텍스 당 3*float
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	////--- attribute 인덱스 1번을 사용 가능하게 함.
	//glEnableVertexAttribArray(1);
}

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기

	make_fragmentShader(); //--- 프래그먼트 세이더 만들기

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");
	// 
	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program 사용하기
	glUseProgram(s_program);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		glutDestroyWindow(0);
		break;
	}
	glutPostRedisplay();
}

void Draw_Cirlce(GLfloat x, GLfloat y)
{
	for (int i = 0; i < 146; i++) {
		Circleshape[i][0] = x + radius * cos(theta); Circleshape[i][1] = y + radius * sin(theta) * dir;
		theta = PI * radian / 180;
		radius += 0.001f;
		radian += 10;
		radian %= 360;

		//std::cout << Circleshape[i][0] << ", " << Circleshape[i][1] << std::endl;
	}

	x += radius * 2;
	radian = 90;

	for (int i = 146; i < 300; i++) {
		theta = PI * radian / 180;
		Circleshape[i][0] = x + radius * -sin(theta); Circleshape[i][1] = y + radius * -cos(theta) * dir;
		radius -= 0.001f;
		radian += 10;
		radian %= 360;
	}
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 600 * sizeof(GLfloat), Circleshape, GL_STATIC_DRAW);
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDTH / 2)) / (WIDTH / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (start) {
			pointcount = 0;
			theta = 0;
			radius = 0;
			radian = 0;
			for (int i = 0; i < 300; i++) {
				Circleshape[i][0] = Circleshape[i][1] = 0;
			}
			rColor = dis(rd);
			gColor = dis(rd);
			bColor = dis(rd);
			if (rColor < 0.5f)
				dir *= -1;
			Draw_Cirlce(real_x, real_y);
			glutTimerFunc(10, Timer, 1);
			//start = !start;
		}

		else if (!start) {

			//start = !start;
		}
	}
	

}

void Timer(int value)
{
	if (pointcount < 300) {
		pointcount++;
		glutTimerFunc(10, Timer, 1);
	}

	glutPostRedisplay(); // 화면 재 출력
	if (!start)
		return;

}

#endif