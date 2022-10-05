#define Pro 5
#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include<random>


#define WIDHT 800
#define HEIGHT 600

#if Pro == 3

//����ü

typedef struct RECTAN {
	GLfloat first_x, first_y, second_x, second_y;
	GLfloat r, g, b;
};

RECTAN Rectan[6];

//�����μ�����
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0, 1);

//�Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void make_rect();

//����
int shapecount = -1;
int select_rect = 0;
bool mouse_button = false;
GLfloat r = dis(gen), g = dis(gen), b = dis(gen);

void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WIDHT, HEIGHT); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized\n";

	glutKeyboardFunc(Keyboard);
	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}

GLvoid drawScene()//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // �������� ��blue�� �� ����
{
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glClearColor(0.5f, 0.5f, 0.5f, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	for (int i = 0; i <= shapecount; i++) {
		glColor3f(Rectan[i].r, Rectan[i].g, Rectan[i].b);
		glRectf(Rectan[i].first_x, Rectan[i].first_y, Rectan[i].second_x, Rectan[i].second_y);
	}

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� {
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
			std::cout << "�簢�� ���� �ʰ�" << std::endl;
		break;
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
				std::cout << "���õ� �簢�� : " << select_rect << std::endl;
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
		std::cout << Rectan[select_rect].first_x << " " << Rectan[select_rect].first_y << " " << Rectan[select_rect].second_x << std::endl;
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
		std::cout << "�簢�� ����" << std::endl;
}

#elif Pro == 4

typedef struct RECTAN {
	GLfloat first_x, first_y, second_x, second_y;
	GLfloat r, g, b;
};

RECTAN Rectan[6];
RECTAN saveRectan[6];


//�Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

void Mouse(int button, int state, int x, int y);
void TimerFunction(int value);


//�����μ�����
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(0, 1);


//������
int shapecount = -1; //��������
bool animation_a = false, animation_i = false, animation_c = false; //�ִϸ��̼� on/off
GLfloat move_x[5], move_y[5], size_x[5], size_y[5], zigzag[5]; // �ִϸ��̼� ���� / ũ�� ����
unsigned char presskey; //s����
bool onetime = true;

GLfloat r = dis(gen), g = dis(gen), b = dis(gen);
GLfloat back_r = 0.4f, back_g = 0.4f, back_b = 0.4f;

void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(0, 0); // �������� ��ġ ����
	glutInitWindowSize(WIDHT, HEIGHT); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}

	else
		std::cout << "GLEW Initialized\n";

	for (int i = 0; i < 5; i++) {
		move_x[i] = 0.005f;
		move_y[i] = 0.005f;
		size_x[i] = 0.003f;
		size_y[i] = 0.003f;
		zigzag[i] = 0.2f;
		std::cout << move_x[i] << " " << move_y[i] << std::endl;
	}

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	//glutTimerFunc(100, TimerFunction, 1);
	glutMainLoop(); // �̺�Ʈ ó�� ���� }

}

GLvoid drawScene()//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // �������� ��blue�� �� ����
{
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glClearColor(back_r, back_g, back_b, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	
	for (int i = 0; i <= shapecount; i++) {
		glColor3f(Rectan[i].r, Rectan[i].g, Rectan[i].b);
		glRectf(Rectan[i].first_x, Rectan[i].first_y, Rectan[i].second_x, Rectan[i].second_y);
	}
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
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
			std::cout << "�簢���� �ִ� 5�������Դϴ�.)" << std::endl;
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

#elif Pro == 5
#include <cmath>
//����ü

typedef struct RECTAN {
	GLfloat first_x, first_y, second_x, second_y;
	GLfloat r, g, b;
};

RECTAN bg_rect[31];
RECTAN main_rect;

//�����μ�����
std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<GLfloat> dis(-0.9, 0.9);

//�Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

//����
int shapecount = 29;
bool mouse_button = false;
GLfloat r = dis(gen), g = dis(gen), b = dis(gen);

void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� { //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WIDHT, HEIGHT); // �������� ũ�� ����
	glutCreateWindow("Example1"); // ������ ����

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
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

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}

GLvoid drawScene()//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // �������� ��blue�� �� ����
{
	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
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

	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� {
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

#endif