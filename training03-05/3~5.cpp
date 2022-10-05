#define Training 5
#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include<random>


#define WIDHT 800
#define HEIGHT 600

#if Training == 3

//구조체

typedef struct RECTAN {
	GLfloat first_x, first_y, second_x, second_y;
	GLfloat r, g, b;
};

RECTAN Rectan[6];

//랜덤인수설정
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0, 1);

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void make_rect();

//변수
int shapecount = -1;
int select_rect = 0;
bool mouse_button = false;
GLfloat r = dis(gen), g = dis(gen), b = dis(gen);

void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WIDHT, HEIGHT); // 윈도우의 크기 지정
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

	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glClearColor(1.0f, 1.0f, 1.0f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i <= shapecount; i++) {
		glColor3f(Rectan[i].r, Rectan[i].g, Rectan[i].b);
		glRectf(Rectan[i].first_x, Rectan[i].first_y, Rectan[i].second_x, Rectan[i].second_y);
	}

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
{
	glViewport(0, 0, w, h);
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key) {
	case 'a':
		if (shapecount < 4) {
			shapecount++;
			make_rect();
			r = dis(gen);
			g = dis(gen);
			b = dis(gen);
		}
		else
			std::cout << "사각형 개수 초과" << std::endl;
		break;
	case 'q':
		exit(0);
	}
	glutPostRedisplay();
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDHT / 2)) / (WIDHT / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		for (int i = shapecount; i >= 0; i--) {
			if (Rectan[i].first_x < real_x && Rectan[i].first_y < real_y && Rectan[i].second_x > real_x && Rectan[i].second_y > real_y) {
				mouse_button = true;
				select_rect = i;
				//std::cout << "선택된 사각형 : " << select_rect << std::endl;
				break;
			}
		}
	}

	else if (state == GLUT_UP) {
		mouse_button = false;
		select_rect = -1;
	}
}


void Motion(int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDHT / 2)) / (WIDHT / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (mouse_button == true) {
		Rectan[select_rect].first_x = real_x - 0.2f;
		Rectan[select_rect].first_y = real_y - 0.2f;
		Rectan[select_rect].second_x = real_x + 0.2f;
		Rectan[select_rect].second_y = real_y + 0.2f;
		//std::cout << Rectan[select_rect].first_x << " " << Rectan[select_rect].first_y << " " << Rectan[select_rect].second_x << std::endl;
	}
	glutPostRedisplay();
}

void make_rect()
{
		Rectan[shapecount].r = r; Rectan[shapecount].g = g; Rectan[shapecount].b = b;
		Rectan[shapecount].first_x = -0.2f;
		Rectan[shapecount].first_y = -0.2f;
		Rectan[shapecount].second_x = 0.2f;
		Rectan[shapecount].second_y = 0.2f;
		//std::cout << "사각형 생성" << std::endl;
}

#elif Training == 4

typedef struct RECTAN {
	GLfloat first_x, first_y, second_x, second_y;
	GLfloat r, g, b;
	int zigzag_count;
};

RECTAN Rectan[6];
RECTAN saveRectan[6];


//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);


//랜덤인수설정
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0, 1);


//변수들
int shapecount = -1; //도형개수
bool animation_a = false, animation_i = false, animation_c = false; //애니메이션 on/off
GLfloat move_x[5], move_y[5], size_x[5], size_y[5], zigzag[5]; // 애니메이션 상하 / 크기 변수
unsigned char presskey; //s정지
bool onetime = true;

GLfloat r = dis(gen), g = dis(gen), b = dis(gen);
GLfloat back_r = 1.0f, back_g = 1.0f, back_b = 1.0f;

void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(0, 0); // 윈도우의 위치 지정
	glutInitWindowSize(WIDHT, HEIGHT); // 윈도우의 크기 지정
	glutCreateWindow("Example1"); // 윈도우 생성

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		//std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		//std::cout << "GLEW Initialized\n";

	for (int i = 0; i < 5; i++) {
		move_x[i] = 0.005f;
		move_y[i] = 0.005f;
		size_x[i] = 0.003f;
		size_y[i] = 0.003f;
		zigzag[i] = 0.2f;
		//std::cout << move_x[i] << " " << move_y[i] << std::endl;
	}

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	//glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop(); // 이벤트 처리 시작 }

}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glClearColor(back_r, back_g, back_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	
	for (int i = 0; i <= shapecount; i++) {
		glColor3f(Rectan[i].r, Rectan[i].g, Rectan[i].b);
		glRectf(Rectan[i].first_x, Rectan[i].first_y, Rectan[i].second_x, Rectan[i].second_y);
	}
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDHT / 2)) / (WIDHT / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (shapecount < 4) {
			shapecount++;
			r = dis(gen); Rectan[shapecount].r = r;
			g = dis(gen); Rectan[shapecount].g = g;
			b = dis(gen); Rectan[shapecount].b = b;
			Rectan[shapecount].first_x = real_x - 0.2f;
			Rectan[shapecount].first_y = real_y - 0.2f;
			Rectan[shapecount].second_x = real_x + 0.2f;
			Rectan[shapecount].second_y = real_y + 0.2f;
			saveRectan[shapecount] = Rectan[shapecount];
		}

		else
			std::cout << "사각형은 최대 5개까지입니다.)" << std::endl;
	}
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
	presskey = key;
	switch (key) {
	case 'a':
		if (animation_a == false) {
			animation_i = false;
			animation_a = true;
			glutTimerFunc(10, TimerFunction, 1);
		}

		else if (animation_a == true)
			animation_a = false;
			key = 's';
		break;

	case 'i':
		if (animation_i == false) {
			animation_a = false;
			animation_i = true;
			glutTimerFunc(10, TimerFunction, 1);
		}


		else if (animation_i == true)
			animation_i = false;
			key = 's';
		break;

	case 'c':
		if (animation_c == false) {
			animation_c = true;
			glutTimerFunc(10, TimerFunction, 1);
		}

		else if (animation_c == true) {
			animation_c = false;
			key = 's';
		}
		break;


	case 's':
		animation_a = animation_c = animation_i = false;
		break;

	case 'r':
		shapecount = -1;
		break;

	case 'm':
		for (int i = 0; i <= shapecount; i++) {
			Rectan[i] = saveRectan[i];
		}
		break;

	case 'q':
		glutLeaveMainLoop();
		break;
	}
	glutPostRedisplay();
}

void TimerFunction(int value)
{
	if (animation_a == true) {
		for (int i = 0; i <= shapecount; i++) {
			Rectan[i].first_x += move_x[i];
			Rectan[i].first_y += move_y[i];
			Rectan[i].second_x += move_x[i];
			Rectan[i].second_y += move_y[i];
		}

		for (int i = 0; i <= shapecount; i++) {
			if (Rectan[i].first_x <= -1.f || Rectan[i].second_x > 1.f)
				move_x[i] *= -1;
			if (Rectan[i].first_y <= -1.f || Rectan[i].second_y > 1.f)
				move_y[i] *= -1;
		}
	}

	if (animation_i == true) {
		for (int i = 0; i <= shapecount; i++) {
			Rectan[i].first_x += move_x[i];
			Rectan[i].second_x += move_x[i];
		}

		for (int i = 0; i <= shapecount; i++) {
			if (Rectan[i].first_x <= -1.f || Rectan[i].second_x > 1.f) {
				move_x[i] *= -1;
				Rectan[i].first_y += zigzag[i]; Rectan[i].second_y += zigzag[i];
			}
			if ((Rectan[i].first_y <= -1.1f || Rectan[i].second_y >= 1.1f) && (Rectan[i].first_x <= -1.f || Rectan[i].second_x > 1.f))
				zigzag[i] *= -1;
		}
	}

	if (animation_c == true) {
		for (int i = 0; i <= shapecount; i++) {
			Rectan[i].first_x -= size_x[i];
			Rectan[i].first_y -= size_y[i];
			Rectan[i].second_x += size_x[i];
			Rectan[i].second_y += size_y[i];
		}

		for (int i = 0; i <= shapecount; i++) {
			if (Rectan[i].second_x - Rectan[i].first_x > 0.7f || Rectan[i].second_x - Rectan[i].first_x < 0.f)
				size_x[i] *= -1;
			if (Rectan[i].second_y - Rectan[i].first_y > 0.7f || Rectan[i].second_y - Rectan[i].first_y < 0.f)
				size_y[i] *= -1;
		}
	}

	glutPostRedisplay();

	
	if (presskey != 's') {
		if(animation_a == true || animation_i == true || animation_c == true)
			glutTimerFunc(10, TimerFunction, 1);
	}
}

#elif Training == 5
#include <cmath>
//구조체

typedef struct RECTAN {
	GLfloat first_x, first_y, second_x, second_y;
	GLfloat r, g, b;
};

RECTAN bg_rect[31];
RECTAN main_rect;

//랜덤인수설정
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(-0.9, 0.9);

//함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void Recreate();


//변수
int shapecount = 29;
bool mouse_button = false;
GLfloat r = dis(gen), g = dis(gen), b = dis(gen);

void main(int argc, char** argv) { //--- 윈도우 출력하고 콜백함수 설정 { //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(100, 100); // 윈도우의 위치 지정
	glutInitWindowSize(WIDHT, HEIGHT); // 윈도우의 크기 지정
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

	for (int i = 1; i <= shapecount; i++) {
		r = abs(dis(gen));
		g = abs(dis(gen));
		b = abs(dis(gen));

		bg_rect[i].r = r; bg_rect[i].g = g; bg_rect[i].b = b;
		bg_rect[i].first_x = dis(gen);
		bg_rect[i].first_y = dis(gen);
		bg_rect[i].second_x = bg_rect[i].first_x + 0.1f; bg_rect[i].second_y = bg_rect[i].first_y + 0.1f;

	}

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // 이벤트 처리 시작 }
}

GLvoid drawScene()//--- 콜백 함수: 그리기 콜백 함수 { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
{
	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glClearColor(1.f, 1.f, 1.f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	if (mouse_button == true) {
		main_rect.r = 0.f; main_rect.g = 0.f; main_rect.b = 0.f;
		glColor3f(main_rect.r, main_rect.g, main_rect.b);
		glRectf(main_rect.first_x, main_rect.first_y, main_rect.second_x, main_rect.second_y);
	}

	for (int i = 1; i <= shapecount; i++) {
		glColor3f(bg_rect[i].r, bg_rect[i].g, bg_rect[i].b);
		glRectf(bg_rect[i].first_x, bg_rect[i].first_y, bg_rect[i].second_x, bg_rect[i].second_y);
		//std::cout << bg_rect[i].first_y << " " << std::endl;
	}

	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수 {
{
	glViewport(0, 0, w, h);
}

void Mouse(int button, int state, int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDHT / 2)) / (WIDHT / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		mouse_button = true;
		std::cout << "true" << std::endl;
	}

	else if (state == GLUT_UP) {
		mouse_button = false;
	}
	glutPostRedisplay();
}


void Motion(int x, int y)
{
	GLfloat real_x = (GLfloat)(x - (WIDHT / 2)) / (WIDHT / 2);
	GLfloat real_y = (GLfloat)-(y - (HEIGHT / 2)) / (HEIGHT / 2);

	if (mouse_button == true) {
		main_rect.first_x = real_x - 0.1f;
		main_rect.first_y = real_y - 0.1f;
		main_rect.second_x = real_x + 0.1f;
		main_rect.second_y = real_y + 0.1f;
		for (int i = 1; i <= shapecount; i++) {
			if(main_rect.first_x < bg_rect[i].second_x &&
				main_rect.first_y < bg_rect[i].second_y && 
				main_rect.second_x > bg_rect[i].first_x &&
				main_rect.second_y > bg_rect[i].first_y){ 

				bg_rect[i].first_x = bg_rect[i].first_y = bg_rect[i].second_x = bg_rect[i].second_y = 0;
			}
		}
	}
	glutPostRedisplay();
}
void Recreate() {
	for (int i = 1; i <= shapecount; i++) {
		r = abs(dis(gen));
		g = abs(dis(gen));
		b = abs(dis(gen));

		bg_rect[i].r = r; bg_rect[i].g = g; bg_rect[i].b = b;
		bg_rect[i].first_x = dis(gen);
		bg_rect[i].first_y = dis(gen);
		bg_rect[i].second_x = bg_rect[i].first_x + 0.1f; bg_rect[i].second_y = bg_rect[i].first_y + 0.1f;

	}
	glutPostRedisplay();
}

GLvoid Keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'r':
		Recreate();
		break;
	case 'q':
		exit(0);
	}

	glutPostRedisplay();
}

#endif