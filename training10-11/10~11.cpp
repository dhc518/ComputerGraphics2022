#define Pro 10
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "file.h"
#include <random>
#include <windows.h>

#define WIDTH 800
#define HEIGHT 600

#if Pro == 10

//����
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

//�Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShaders();
void make_fragmentShaders();
void InitBuffer();
void InitShader();

//�׸��� �Լ�
void draw_middle_line();
void draw_line();
void draw_triangle();
void draw_rectangle();
void draw_pentagon();

//�����̱� �Լ�
void change_line();
void change_triangle();
void change_rectangle();
void change_pentagon();

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);


void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WIDTH, HEIGHT); // �������� ũ�� ����
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

	draw_middle_line();
	draw_line();
	draw_triangle();
	draw_rectangle();
	draw_pentagon();

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}

GLvoid drawScene()//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // �������� ��blue�� �� ����
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);

	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao[5]);
	glEnableVertexAttribArray(0);


	glUniform3f(vColorLocation, 0, 0, 0);
	//--- �ﰢ�� �׸���
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

	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�

}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� {
{
	glViewport(0, 0, w, h);
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);

	//--- �������� ����� ���� ���� ���: ���� üũ

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");

	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);

	//--- �������� ����� ���� ���� ���: ������ ���� üũ

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void InitBuffer()
{
	glGenVertexArrays(6, vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(6, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao[0]); //--- VAO�� ���ε��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(GLfloat), point_shape, GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
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

	////--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(0);

}

void InitShader()
{
	make_vertexShader(); //--- ���ؽ� ���̴� �����
	make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");
	// 
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program ����ϱ�
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

	glutPostRedisplay(); // ȭ�� �� ���

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
	//���μ�
	middle_line[0][0] = -1.f;   middle_line[1][0] = 1.f;  
	middle_line[0][1] = middle_line[1][1] = 0; 

	//���μ�
	middle_line[2][0] = middle_line[3][0] = 0;
	middle_line[2][1] = 1.f;  middle_line[3][1] = -1.f;
}

void draw_point()
{

}

void draw_line()
{
	line_shape[0][0] = -0.8f;  line_shape[0][1] = 0.2f;  //���� ��
	line_shape[1][0] = -0.2f;  line_shape[1][1] = 0.8f;  //������ ��
	line_shape[2][0] = -0.2f;  line_shape[2][1] = 0.8f;  //�����ִ� ��
}

void draw_triangle()
{
	//�����ﰢ�� 2���� ����
	//ù��°�ﰢ�� (��)
	tri_shape[0][0] = 0.2f;  tri_shape[0][1] = 0.8f;  //ù��° ��(���� ���)
	tri_shape[1][0] = 0.8f;  tri_shape[1][1] = 0.8f;  //�ι�° ��(���� ���)
	tri_shape[2][0] = 0.2f;  tri_shape[2][1] = 0.2f;  //����° ��(���� �ϴ�)

	//�ι�° �ﰢ��(�Ʒ�)
	tri_shape[3][0] = 0.8f;  tri_shape[3][1] = 0.8f;  //�׹�° ��(���� ���) -> �ι�° ���� ��ħ
	tri_shape[4][0] = 0.2f;  tri_shape[4][1] = 0.2f;  //�ټ���° �� (���� �ϴ�) -> ����° ���� ��ħ
	tri_shape[5][0] = 0.8f;  tri_shape[5][1] = 0.8f;  //������° �� (���� �ϴ�)

}

void draw_rectangle()
{
	//ù��° �ﰢ��
	rect_shape[0][0] = -0.5f;  rect_shape[0][1] = -0.4f;  //ù��° ��(�������� �����)
	rect_shape[1][0] = -0.8f;  rect_shape[1][1] = -0.4f;  //�ι�° ��(�������� ����)
	rect_shape[2][0] = -0.8f;  rect_shape[2][1] = -0.9f;  //������ ��(�������� ���� �ϴ�)

	//�ι�° �ﰢ��
	rect_shape[3][0] = -0.5f;  rect_shape[3][1] = -0.4f;  //�׹�° ��(ù��° ���� ��ħ)
	rect_shape[4][0] = -0.8f;  rect_shape[4][1] = -0.9f;  //�ټ���° ��(����° ���� ��ħ)
	rect_shape[5][0] = -0.2f;  rect_shape[5][1] = -0.9f;  //������° ��(�������� ���� �ϴ�)

	//����° �ﰢ��
	rect_shape[6][0] = -0.5f;  rect_shape[6][1] = -0.4f;  //�ϰ���° ��(ù��° ���� ��ħ)
	rect_shape[7][0] = -0.2f;  rect_shape[7][1] = -0.9f;  //������° ��(������° ���� ��ħ)
	rect_shape[8][0] = -0.2f;  rect_shape[8][1] = -0.4f;  //��ȩ��° ��(�������� ����)
}

void draw_pentagon()
{
	//ù��° �ﰢ��
	penta_shape[0][0] = 0.5f;  penta_shape[0][1] = -0.1f;  //ù��° ��(�������� �����)
	penta_shape[1][0] = 0.2f;  penta_shape[1][1] = -0.4f;  //�ι�° ��(�������� ����)
	penta_shape[2][0] = 0.3f;  penta_shape[2][1] = -0.9f;  //������ ��(�������� ���� �ϴ�)

	//�ι�° �ﰢ��
	penta_shape[3][0] = 0.5f;  penta_shape[3][1] = -0.1f;  //�׹�° ��(ù��° ���� ��ħ)
	penta_shape[4][0] = 0.3f;  penta_shape[4][1] = -0.9f;  //�ټ���° ��(����° ���� ��ħ)
	penta_shape[5][0] = 0.7f;  penta_shape[5][1] = -0.9f;  //������° ��(�������� ���� �ϴ�)

	//����° �ﰢ��
	penta_shape[6][0] = 0.5f;  penta_shape[6][1] = -0.1f;  //�ϰ���° ��(ù��° ���� ��ħ)
	penta_shape[7][0] = 0.7f;  penta_shape[7][1] = -0.9f;  //������° ��(������° ���� ��ħ)
	penta_shape[8][0] = 0.8f;  penta_shape[8][1] = -0.4f;  //��ȩ��° ��(�������� ����)

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
	glutPostRedisplay(); // ȭ�� �� ���
}

void change_line()
{
	line_shape[2][1] -= line_move;
	
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(GLfloat), line_shape, GL_STATIC_DRAW);
	glutPostRedisplay(); // ȭ�� �� ���
}

void change_triangle()
{
	tri_shape[5][1] += triangle_move;

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), tri_shape, GL_STATIC_DRAW);
	glutPostRedisplay(); // ȭ�� �� ���
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
	glutPostRedisplay(); // ȭ�� �� ���
}



#elif Pro == 11

//����
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

//�Լ�
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


void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(100, 100); // �������� ��ġ ����
	glutInitWindowSize(WIDTH, HEIGHT); // �������� ũ�� ����
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



	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}

GLvoid drawScene()//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // �������� ��blue�� �� ����
{
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);
	//glClearColor(1.0, 1.0, 1.0, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);

	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);

	glPointSize(5);
	glDrawArrays(GL_POINTS, 0, 4);

	glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);
	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�

	draw_point_rect();

}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� {
{
	glViewport(0, 0, w, h);
}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex.glsl");

	//--- ���ؽ� ���̴� ��ü �����
	vertexshader = glCreateShader(GL_VERTEX_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(vertexshader, 1, (const GLchar**)&vertexsource, 0);

	//--- ���ؽ� ���̴� �������ϱ�
	glCompileShader(vertexshader);

	//--- �������� ����� ���� ���� ���: ���� üũ

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(vertexshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexshader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void make_fragmentShader()
{
	fragmentsource = filetobuf("fragment.glsl");

	//--- �����׸�Ʈ ���̴� ��ü �����
	fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);

	//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
	glShaderSource(fragmentshader, 1, (const GLchar**)&fragmentsource, 0);

	//--- �����׸�Ʈ ���̴� ������
	glCompileShader(fragmentshader);

	//--- �������� ����� ���� ���� ���: ������ ���� üũ

	GLint result;
	GLchar errorLog[512];

	glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentshader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
		return;
	}
}

void InitBuffer()
{
	glGenVertexArrays(1, &vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�

	glBindVertexArray(vao); //--- VAO�� ���ε��ϱ�

	glGenBuffers(1, &vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(1, &ebo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�

	//--- 1��° VBO�� Ȱ��ȭ�Ͽ� ���ε��ϰ�, ���ؽ� �Ӽ� (��ǥ��)�� ����
	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	//--- triShape �迭�� ������: 9 * float
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), Linevertex, GL_STATIC_DRAW);

	//--- ��ǥ���� attribute �ε��� 0���� ����Ѵ�: ���ؽ� �� 3* float
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);

	////--- ������ attribute �ε��� 1���� ����Ѵ�: ���ؽ� �� 3*float
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * sizeof(unsigned int), rectshape, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, 0);

	////--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(0);

}

void InitShader()
{
	make_vertexShader(); //--- ���ؽ� ���̴� �����

	make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����

	//-- shader Program
	s_program = glCreateProgram();
	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);
	glLinkProgram(s_program);

	//checkCompileErrors(s_program, "PROGRAM");
	// 
	//--- ���̴� �����ϱ�
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

	//--- Shader Program ����ϱ�
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
		//1��и� �� üũ
		if (real_x > point_rect[0][0][0] && real_x < point_rect[0][1][0] && real_y > point_rect[0][0][1] && real_y < point_rect[0][1][1]) {
			Linevertex[0][0] = real_x;
			Linevertex[0][1] = real_y;
		}

		//2��и� �� üũ
		else if (real_x > point_rect[1][0][0] && real_x < point_rect[1][1][0] && real_y > point_rect[1][0][1] && real_y < point_rect[1][1][1]) {
			Linevertex[1][0] = real_x;
			Linevertex[1][1] = real_y;
		}

		//3��и� �� üũ
		else if (real_x > point_rect[2][0][0] && real_x < point_rect[2][1][0] && real_y > point_rect[2][0][1] && real_y < point_rect[2][1][1]) {
			Linevertex[2][0] = real_x;
			Linevertex[2][1] = real_y;
		}

		//4��и� �� üũ
		else if (real_x > point_rect[3][0][0] && real_x < point_rect[3][1][0] && real_y > point_rect[3][0][1] && real_y < point_rect[3][1][1]) {
			Linevertex[3][0] = real_x;
			Linevertex[3][1] = real_y;
		}

		//�簢�� ���� üũ
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
	//���� ��� �� �浹üũ(1��и�)
	point_rect[0][0][0] = Linevertex[0][0] - 0.1f;
	point_rect[0][0][1] = Linevertex[0][1] - 0.1f;  //p1 ���� �ϴ�

	point_rect[0][1][0] = Linevertex[0][0] + 0.1f;
	point_rect[0][1][1] = Linevertex[0][1] + 0.1f;  //p2 ���� ���

	//���� ��� �� �浹üũ(2��и�)
	point_rect[1][0][0] = Linevertex[1][0] - 0.1f;
	point_rect[1][0][1] = Linevertex[1][1] - 0.1f;  //p1 ���� �ϴ�

	point_rect[1][1][0] = Linevertex[1][0] + 0.1f;
	point_rect[1][1][1] = Linevertex[1][1] + 0.1f;  //p2 ���� ���

	//���� �ϴ� �� �浹üũ(3��и�)
	point_rect[2][0][0] = Linevertex[2][0] - 0.1f;
	point_rect[2][0][1] = Linevertex[2][1] - 0.1f;  //p1 ���� �ϴ�

	point_rect[2][1][0] = Linevertex[2][0] + 0.1f;
	point_rect[2][1][1] = Linevertex[2][1] + 0.1f;  //p2 ���� ���

	//���� �ϴ� �� �浹üũ(4��и�)
	point_rect[3][0][0] = Linevertex[3][0] - 0.1f;
	point_rect[3][0][1] = Linevertex[3][1] - 0.1f;  //p1 ���� �ϴ�

	point_rect[3][1][0] = Linevertex[3][0] + 0.1f;
	point_rect[3][1][1] = Linevertex[3][1] + 0.1f;  //p2 ���� ���
}

#endif