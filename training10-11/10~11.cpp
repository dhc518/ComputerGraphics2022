#define Pro 10
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "file.h"
#include <random>
#include <windows.h>

#define WIDTH 800
#define HEIGHT 600

#if Pro == 10

//변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao[6], vbo[6];
GLuint s_program, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat point_shape[2] = {};
GLfloat line_shape[3][2] = {};
GLfloat middle_line[4][2] = {};
GLfloat tri_shape[6][2] = {};
GLfloat rect_shape[9][2] = {};
GLfloat penta_shape[9][2] = {};

GLfloat colors[4][3] = {};

GLfloat line_move = 0.06f;
GLfloat triangle_move = -0.06f;
GLfloat rect_move_x = 0.01f;
GLfloat rect_move_y = 0.03f;
GLfloat penta_move_x = 0.03f;
GLfloat penta_move_y = 0.04f;


bool change = false;
int animation = 0;

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();

//그리기 함수
void draw_middle_line();
void draw_line();
void draw_triangle();
void draw_rectangle();
void draw_pentagon();

//움직이기 함수
void change_line();
void change_triangle();
void change_rectangle();
void change_pentagon();

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

	draw_middle_line();
	draw_line();
	draw_triangle();
	draw_rectangle();
	draw_pentagon();

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
	glBindVertexArray(vao[5]);
	glEnableVertexAttribArray(0);


	glUniform3f(vColorLocation, 0, 0, 0);
	//--- 삼각형 그리기
	for (int i = 0; i < 4; i++) {
		glDrawArrays(GL_LINES, i * 2, 2);
	}

	glUniform3f(vColorLocation, 0, 0, 1.f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if(animation > 0)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	
	glBindVertexArray(vao[1]);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glUniform3f(vColorLocation, 1.f, 1.f, 0);
	glBindVertexArray(vao[2]);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);


	glUniform3f(vColorLocation, 0, 1.f, 0);
	glBindVertexArray(vao[3]);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 9);

	glUniform3f(vColorLocation, 1.f, 0, 0);

	if (animation == 10) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
		glPointSize(5);
	}

	glBindVertexArray(vao[4]);
	glEnableVertexAttribArray(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 9);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glPointSize(1);

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
	glGenVertexArrays(6, vao); //--- VAO 를 지정하고 할당하기
	glGenBuffers(6, vbo); //--- 2개의 VBO를 지정하고 할당하기

	glBindVertexArray(vao[0]); //--- VAO를 바인드하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat), point_shape, GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute 인덱스 0번을 사용가능하게 함
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), line_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tri_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), rect_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), penta_shape, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindVertexArray(vao[5]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[5]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), middle_line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

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

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 's':
		change = !change;

		glutTimerFunc(100, Timer, 1);
		break;
	}
	glutPostRedisplay();
}

void Timer(int value)
{
	change_line();
	change_triangle();
	change_rectangle();
	change_pentagon();

	if (change)
		animation += 1;

	glutPostRedisplay(); // 화면 재 출력

	if (animation == 10) {
		line_move *= -1;
		triangle_move *= -1;
		rect_move_x *= -1;
		rect_move_y *= -1;
		penta_move_x *= -1;
		penta_move_y *= -1;
	}

	if (animation == 11)
		Sleep(700);

	if (animation == 20) {
		change = !change;
		line_move *= -1;
		triangle_move *= -1;
		rect_move_x *= -1;
		rect_move_y *= -1;
		penta_move_x *= -1;
		penta_move_y *= -1;
		animation = 0;
		return;
	}

	glutTimerFunc(100, Timer, 1);
}

void draw_middle_line()
{
	//가로선
	middle_line[0][0] = -1.f;   middle_line[1][0] = 1.f;  
	middle_line[0][1] = middle_line[1][1] = 0; 

	//세로선
	middle_line[2][0] = middle_line[3][0] = 0;
	middle_line[2][1] = 1.f;  middle_line[3][1] = -1.f;
}

void draw_point()
{

}

void draw_line()
{
	line_shape[0][0] = -0.8f;  line_shape[0][1] = 0.2f;  //왼쪽 점
	line_shape[1][0] = -0.2f;  line_shape[1][1] = 0.8f;  //오른쪽 점
	line_shape[2][0] = -0.2f;  line_shape[2][1] = 0.8f;  //겹쳐있는 점
}

void draw_triangle()
{
	//직각삼각형 2개로 생성
	//첫번째삼각형 (위)
	tri_shape[0][0] = 0.2f;  tri_shape[0][1] = 0.8f;  //첫번째 점(왼쪽 상단)
	tri_shape[1][0] = 0.8f;  tri_shape[1][1] = 0.8f;  //두번째 점(우측 상단)
	tri_shape[2][0] = 0.2f;  tri_shape[2][1] = 0.2f;  //세번째 점(좌측 하단)

	//두번째 삼각형(아래)
	tri_shape[3][0] = 0.8f;  tri_shape[3][1] = 0.8f;  //네번째 점(우측 상단) -> 두번째 점과 겹침
	tri_shape[4][0] = 0.2f;  tri_shape[4][1] = 0.2f;  //다섯번째 점 (좌측 하단) -> 세번째 점과 겹침
	tri_shape[5][0] = 0.8f;  tri_shape[5][1] = 0.8f;  //여섯번째 점 (우측 하단)

}

void draw_rectangle()
{
	//첫번째 삼각형
	rect_shape[0][0] = -0.5f;  rect_shape[0][1] = -0.4f;  //첫번째 점(오각형의 꼭대기)
	rect_shape[1][0] = -0.8f;  rect_shape[1][1] = -0.4f;  //두번째 점(오각형의 좌측)
	rect_shape[2][0] = -0.8f;  rect_shape[2][1] = -0.9f;  //세번쨰 점(오각형의 좌측 하단)

	//두번째 삼각형
	rect_shape[3][0] = -0.5f;  rect_shape[3][1] = -0.4f;  //네번째 점(첫번째 점과 겹침)
	rect_shape[4][0] = -0.8f;  rect_shape[4][1] = -0.9f;  //다섯번째 점(세번째 점과 겹침)
	rect_shape[5][0] = -0.2f;  rect_shape[5][1] = -0.9f;  //여섯번째 점(오각형의 우측 하단)

	//세번째 삼각형
	rect_shape[6][0] = -0.5f;  rect_shape[6][1] = -0.4f;  //일곱번째 점(첫번째 점과 겹침)
	rect_shape[7][0] = -0.2f;  rect_shape[7][1] = -0.9f;  //여덟번째 점(여섯번째 점과 겹침)
	rect_shape[8][0] = -0.2f;  rect_shape[8][1] = -0.4f;  //아홉번째 점(오각형의 우측)
}

void draw_pentagon()
{
	//첫번째 삼각형
	penta_shape[0][0] = 0.5f;  penta_shape[0][1] = -0.1f;  //첫번째 점(오각형의 꼭대기)
	penta_shape[1][0] = 0.2f;  penta_shape[1][1] = -0.4f;  //두번째 점(오각형의 좌측)
	penta_shape[2][0] = 0.3f;  penta_shape[2][1] = -0.9f;  //세번쨰 점(오각형의 좌측 하단)

	//두번째 삼각형
	penta_shape[3][0] = 0.5f;  penta_shape[3][1] = -0.1f;  //네번째 점(첫번째 점과 겹침)
	penta_shape[4][0] = 0.3f;  penta_shape[4][1] = -0.9f;  //다섯번째 점(세번째 점과 겹침)
	penta_shape[5][0] = 0.7f;  penta_shape[5][1] = -0.9f;  //여섯번째 점(오각형의 우측 하단)

	//세번째 삼각형
	penta_shape[6][0] = 0.5f;  penta_shape[6][1] = -0.1f;  //일곱번째 점(첫번째 점과 겹침)
	penta_shape[7][0] = 0.7f;  penta_shape[7][1] = -0.9f;  //여덟번째 점(여섯번째 점과 겹침)
	penta_shape[8][0] = 0.8f;  penta_shape[8][1] = -0.4f;  //아홉번째 점(오각형의 우측)

}

void change_pentagon()
{
	penta_shape[0][1] -= penta_move_y;
	penta_shape[3][1] -= penta_move_y;
	penta_shape[6][1] -= penta_move_y;

	penta_shape[2][1] += penta_move_y;
	penta_shape[4][1] += penta_move_y;
	penta_shape[5][1] += penta_move_y;
	penta_shape[7][1] += penta_move_y;

	penta_shape[1][0] += penta_move_x;
	penta_shape[8][0] -= penta_move_x;

	penta_shape[2][0] += penta_move_x / 3 * 2;
	penta_shape[4][0] += penta_move_x / 3 * 2;
	penta_shape[5][0] -= penta_move_x / 3 * 2;
	penta_shape[7][0] -= penta_move_x / 3 * 2;

	penta_shape[1][1] -= penta_move_y / 4;
	penta_shape[8][1] -= penta_move_y / 4;

	glBindVertexArray(vao[4]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[4]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), penta_shape, GL_STATIC_DRAW);
	glutPostRedisplay(); // 화면 재 출력
}

void change_line()
{
	line_shape[2][1] -= line_move;
	
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), line_shape, GL_STATIC_DRAW);
	glutPostRedisplay(); // 화면 재 출력
}

void change_triangle()
{
	tri_shape[5][1] += triangle_move;

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tri_shape, GL_STATIC_DRAW);
	glutPostRedisplay(); // 화면 재 출력
}

void change_rectangle()
{
	rect_shape[0][1] += rect_move_y;
	rect_shape[3][1] += rect_move_y;
	rect_shape[6][1] += rect_move_y;

	rect_shape[2][0] += rect_move_x;
	rect_shape[4][0] += rect_move_x;

	rect_shape[5][0] -= rect_move_x;
	rect_shape[7][0] -= rect_move_x;

	glBindVertexArray(vao[3]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), rect_shape, GL_STATIC_DRAW);
	glutPostRedisplay(); // 화면 재 출력
}



#elif Pro == 11

//변수
GLchar* vertexsource, * fragmentsource;
GLuint vertexshader, fragmentshader, vao, vbo, ebo;
GLuint s_program, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLfloat colors[4][3] = {};
GLfloat Linevertex[4][2] = { {-0.5f, 0.5f}, {0.5f, 0.5f}, {-0.5f, -0.5f}, {0.5f, -0.5f} };
unsigned int rectshape[8] = { 0, 1, 0, 2, 1, 3, 2, 3 };
GLfloat point_rect[4][2][2] = {0};
GLfloat start_point[2] = { 0 };

bool mouse_button = false;
GLfloat move_point[2];

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();

void draw_point_rect();

void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);


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



	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
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

	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, 4);

	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	glutSwapBuffers(); //--- 화면에 출력하기

	draw_point_rect();

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
	glGenBuffers(1, &ebo); //--- 2개의 VBO를 지정하고 할당하기

	//--- 1번째 VBO를 활성화하여 바인드하고, 버텍스 속성 (좌표값)을 저장
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//--- triShape 배열의 사이즈: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), Linevertex, GL_STATIC_DRAW);

	//--- 좌표값을 attribute 인덱스 0번에 명시한다: 버텍스 당 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

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

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDTH / 2)) / (WIDTH / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouse_button = true;
		start_point[0] = real_x;
		start_point[1] = real_y;
	}

	else if (button == GLUT_UP) {
		mouse_button = false;
	}
}

void Motion(int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDTH / 2)) / (WIDTH / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);
	
	if (mouse_button) {
		//1사분면 점 체크
		if (real_x > point_rect[0][0][0] && real_x < point_rect[0][1][0] && real_y > point_rect[0][0][1] && real_y < point_rect[0][1][1]) {
			Linevertex[0][0] = real_x;
			Linevertex[0][1] = real_y;
		}

		//2사분면 점 체크
		else if (real_x > point_rect[1][0][0] && real_x < point_rect[1][1][0] && real_y > point_rect[1][0][1] && real_y < point_rect[1][1][1]) {
			Linevertex[1][0] = real_x;
			Linevertex[1][1] = real_y;
		}

		//3사분면 점 체크
		else if (real_x > point_rect[2][0][0] && real_x < point_rect[2][1][0] && real_y > point_rect[2][0][1] && real_y < point_rect[2][1][1]) {
			Linevertex[2][0] = real_x;
			Linevertex[2][1] = real_y;
		}

		//4사분면 점 체크
		else if (real_x > point_rect[3][0][0] && real_x < point_rect[3][1][0] && real_y > point_rect[3][0][1] && real_y < point_rect[3][1][1]) {
			Linevertex[3][0] = real_x;
			Linevertex[3][1] = real_y;
		}

		//사각형 내부 체크
		else if (real_x > Linevertex[0][0] && real_y < Linevertex[0][1] && 
			real_x < Linevertex[1][0] && real_y < Linevertex[1][1] && 
			real_x > Linevertex[2][0] && real_y > Linevertex[2][1] && 
			real_x < Linevertex[3][0] && real_y > Linevertex[3][1]) {

			move_point[0] = real_x - start_point[0];
			move_point[1] = real_y - start_point[1];

			for (int i = 0; i < 4; i++) {
				Linevertex[i][0] += move_point[0];
				Linevertex[i][1] += move_point[1];
			}

			start_point[0] = real_x;
			start_point[1] = real_y;
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), Linevertex, GL_STATIC_DRAW);
	glutPostRedisplay();
}

void draw_point_rect()
{
	//좌측 상단 점 충돌체크(1사분면)
	point_rect[0][0][0] = Linevertex[0][0] - 0.1f;
	point_rect[0][0][1] = Linevertex[0][1] - 0.1f;  //p1 좌측 하단

	point_rect[0][1][0] = Linevertex[0][0] + 0.1f;
	point_rect[0][1][1] = Linevertex[0][1] + 0.1f;  //p2 우측 상단

	//우측 상단 점 충돌체크(2사분면)
	point_rect[1][0][0] = Linevertex[1][0] - 0.1f;
	point_rect[1][0][1] = Linevertex[1][1] - 0.1f;  //p1 좌측 하단

	point_rect[1][1][0] = Linevertex[1][0] + 0.1f;
	point_rect[1][1][1] = Linevertex[1][1] + 0.1f;  //p2 우측 상단

	//좌측 하단 점 충돌체크(3사분면)
	point_rect[2][0][0] = Linevertex[2][0] - 0.1f;
	point_rect[2][0][1] = Linevertex[2][1] - 0.1f;  //p1 좌측 하단

	point_rect[2][1][0] = Linevertex[2][0] + 0.1f;
	point_rect[2][1][1] = Linevertex[2][1] + 0.1f;  //p2 우측 상단

	//우측 하단 점 충돌체크(4사분면)
	point_rect[3][0][0] = Linevertex[3][0] - 0.1f;
	point_rect[3][0][1] = Linevertex[3][1] - 0.1f;  //p1 좌측 하단

	point_rect[3][1][0] = Linevertex[3][0] + 0.1f;
	point_rect[3][1][1] = Linevertex[3][1] + 0.1f;  //p2 우측 상단
}

#endif