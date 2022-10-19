#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include<gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include "file.h"
#include <random>
#include <windows.h>
#include <stdio.h>
#include <string.h>
//#include <gl/glm/glm.hpp>
//#include <gl/glm/ext.hpp>
//#include <gl/glm/gtc/matrix_transform.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/ext.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>

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

//����
GLchar* vertexsource[2], * fragmentsource[2];
GLuint vertexshader[2], fragmentshader[2], vao[3], vbo[3], ebo[2];
GLuint s_program, s_program_plat, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

//glm::vec3 middle_line[4] = {};
GLfloat middle_line[4][3] = {};

READ cube;
READ pyramid;

bool cube_on = false;
bool pyramid_on = false;
bool grain = false;
bool x_on = false;
bool y_on = false;
bool draw_type = true;

float x_rotation = 0;
float y_rotation = 0;

float x_trans = 0;
float y_trans = 0;

//�Լ�
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);

void make_vertexShader();
void make_fragmentShader();
void InitBuffer();
void InitShader();

void ReadObj(FILE* objFile, READ& Read);

//�׸��� �Լ�
void draw_middle_line();

void Keyboard(unsigned char key, int x, int y);
void Timer(int value);
void SpecialKeyboard(int key, int x, int y);


void main(int argc, char** argv) { //--- ������ ����ϰ� �ݹ��Լ� ���� //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // ���÷��� ��� ����
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

	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ���� }
}

GLvoid drawScene()//--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� { glClearColor( 0.0f, 0.0f, 1.0f, 1.0f ); // �������� ��blue�� �� ����
{
	int vColorLocation = glGetUniformLocation(s_program_plat, "out_Color");
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program_plat);

	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(0);

	//�߾Ӽ�
	glUniform3f(vColorLocation, 1.f, 0, 0);
	glDrawArrays(GL_LINES, 0, 4);


	//���� �׸���
	if (grain)
		glEnable(GL_DEPTH_TEST);

	else if (!grain)
		glDisable(GL_DEPTH_TEST);

	glUseProgram(s_program);
	vColorLocation = glGetUniformLocation(s_program, "out_Color");
	glm::mat4 model = glm::mat4(1.0f);
	model = translate(model, vec3(x_trans, y_trans, 0.f));
	model = glm::rotate(model, glm::radians(30.0f + x_rotation), glm::vec3(1.0, 0.0, 0.0));
	model = glm::rotate(model, glm::radians(-30.0f + y_rotation), glm::vec3(0.0, 1.0, 0.0));

	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	//--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	if(draw_type)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	else if (!draw_type)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	if (cube_on) {
		glBindVertexArray(vao[1]);
		glEnableVertexAttribArray(0);

		for (int i = 0; i < 6; i++)
		{

			if(i == 0)
				glUniform3f(vColorLocation, 1.f, 0, 0);

			if(i == 2)
				glUniform3f(vColorLocation, 0, 1.f, 0);

			if(i == 4)
				glUniform3f(vColorLocation, 0, 0, 1.f);

			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * i * 6));

		}
	}

	else if (pyramid_on) {
		glBindVertexArray(vao[2]);
		glEnableVertexAttribArray(0);

		for (int i = 0; i < 6; i++)
		{
			if (i == 0)
				glUniform3f(vColorLocation, 1.f, 0, 0);

			if (i == 2)
				glUniform3f(vColorLocation, 0, 1.f, 0);

			if (i == 3)
				glUniform3f(vColorLocation, 0, 0, 1.f);

			if (i == 4)
				glUniform3f(vColorLocation, 0, 1.f, 1.f);

			if (i == 5)
				glUniform3f(vColorLocation, 1.f, 1.f, 0);

			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * i * 3));

		}
	}


	glutSwapBuffers(); //--- ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ� {
{
	glViewport(0, 0, w, h);
}

void make_vertexShader()
{
	vertexsource[0] = filetobuf("vertex.glsl");
	vertexsource[1] = filetobuf("vertex_2d.glsl");

	for (int i = 0; i < 2; i++)
	{
		//--- ���ؽ� ���̴� ��ü �����
		vertexshader[i] = glCreateShader(GL_VERTEX_SHADER);

		//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
		glShaderSource(vertexshader[i], 1, (const GLchar**)&vertexsource[i], 0);

		//--- ���ؽ� ���̴� �������ϱ�
		glCompileShader(vertexshader[i]);

		//--- �������� ����� ���� ���� ���: ���� üũ
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(vertexshader[i], GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(vertexshader[i], 512, NULL, errorLog);
			std::cerr << "ERROR: vertex shader ������ ����\n" << errorLog << std::endl;
			return;
		}
	}
}

void make_fragmentShader()
{
	for (int i = 0; i < 2; i++)
	{
		fragmentsource[i] = filetobuf("fragment.glsl");

		//--- �����׸�Ʈ ���̴� ��ü �����
		fragmentshader[i] = glCreateShader(GL_FRAGMENT_SHADER);

		//--- ���̴� �ڵ带 ���̴� ��ü�� �ֱ�
		glShaderSource(fragmentshader[i], 1, (const GLchar**)&fragmentsource[i], 0);

		//--- �����׸�Ʈ ���̴� ������
		glCompileShader(fragmentshader[i]);

		//--- �������� ����� ���� ���� ���: ������ ���� üũ
		GLint result;
		GLchar errorLog[512];
		glGetShaderiv(fragmentshader[i], GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(fragmentshader[i], 512, NULL, errorLog);
			std::cerr << "ERROR: fragment shader ������ ����\n" << errorLog << std::endl;
			return;
		}
	}
}

void InitBuffer()
{
	glGenVertexArrays(3, vao); //--- VAO �� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(3, vbo); //--- 2���� VBO�� �����ϰ� �Ҵ��ϱ�
	glGenBuffers(2, ebo);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(GLfloat), middle_line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//������ü
	FILE* fp;
	fp = fopen("cube.obj", "rb");
	ReadObj(fp, cube);
	for (int i = 0; i < 12; i++)
	{
		cout << cube.face[i].a << "," << cube.face[i].b << ", " << cube.face[i].c << endl;
	}
	cout << endl;
	for (int i = 0; i < 8; i++)
	{
		cout << cube.vertex[i].x << "," << cube.vertex[i].y << ", " << cube.vertex[i].z << endl;
	}
	cout << endl;
	cout << sizeof(FACE) << endl;
	glBindVertexArray(vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(vec3), cube.vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 12 * sizeof(FACE), cube.face, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	//�簢��
	fp = fopen("pyramid.obj", "rb");
	ReadObj(fp, pyramid);

	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), pyramid.vertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(int), pyramid.face, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);

	////--- attribute �ε��� 1���� ��� �����ϰ� ��.
	glEnableVertexAttribArray(0);
}

void InitShader()
{
	make_vertexShader(); //--- ���ؽ� ���̴� �����
	make_fragmentShader(); //--- �����׸�Ʈ ���̴� �����
	//-- shader Program
	s_program = glCreateProgram();
	s_program_plat = glCreateProgram();

	glAttachShader(s_program, vertexshader[0]);
	glAttachShader(s_program, fragmentshader[0]);

	glAttachShader(s_program_plat, vertexshader[1]);
	glAttachShader(s_program_plat, fragmentshader[1]);

	//--- ���̴� �����ϱ�
	for (int i = 0; i < 2; i++)
	{
		glDeleteShader(vertexshader[i]);
		glDeleteShader(fragmentshader[i]);
	}

	glLinkProgram(s_program);

	// ---���̴��� �� ����Ǿ����� üũ�ϱ�
	GLint result;
	GLchar errorLog[512];
	glGetProgramiv(s_program, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(s_program, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return;
	}

	glLinkProgram(s_program_plat);

	// ---���̴��� �� ����Ǿ����� üũ�ϱ�
	glGetProgramiv(s_program_plat, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(s_program_plat, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		return;
	}
}

void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'c':
		pyramid_on = false;
		cube_on = true;	
		break;

	case 'p':
		cube_on = false;
		pyramid_on = true;
		break;

	case 'h':
		grain = !grain;
		break;

	case 'x':
		x_on = !x_on;
		glutTimerFunc(10, Timer, 1);
		break;

	case 'y':
		y_on = !y_on;
		glutTimerFunc(10, Timer, 1);
		break;

	case 's':
		x_on = false;
		y_on = false;
		grain = true;
		x_trans = 0;
		y_trans = 0;
		x_rotation = y_rotation = 0;
		break;

	case 'w':
		draw_type = !draw_type;
		break;

	case 'q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_UP) {
		y_trans += 0.01f;
	}

	else if (key == GLUT_KEY_DOWN) {
		y_trans -= 0.01f;
	}

	else if (key == GLUT_KEY_LEFT) {
		x_trans -= 0.01f;
	}

	else if (key == GLUT_KEY_RIGHT) {
		x_trans += 0.01f;
	}
	glutPostRedisplay();
}

void Timer(int value)
{
	if (x_on) {
		x_rotation += 1;
	}

	if (y_on) {
		y_rotation += 1;
	}

	else if (!x_on && !y_on) {
		return;
	}

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
	//middle_line[0] = {1.f, 0, 0};  //������ ��
	//middle_line[1] = {-1.f, 0, 0 };  //���� ��
	//middle_line[2] = { 0, 1.f, 0 };  //���� ��
	//middle_line[3] = { 0, -1.f, 0 };  //�Ʒ��� ��
	//std::cout << "��" << std::endl;

	middle_line[0][0] = 1.f;  middle_line[0][1] = 0;  middle_line[0][2] = 0;
	middle_line[1][0] = -1.f;  middle_line[1][1] = 0;  middle_line[1][2] = 0;
	middle_line[2][0] = 0;  middle_line[2][1] = 1.f;  middle_line[2][2] = 0;
	middle_line[3][0] = 0;  middle_line[3][1] = -1.f;  middle_line[3][2] = 0;
}

void ReadObj(FILE* objFile, READ& Read)
{
	//--- 1. ��ü ���ؽ� ���� �� �ﰢ�� ���� ����
	char count[100];
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			Read.vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			Read.faceNum += 1;
		memset(count, '\0', sizeof(count)); // �迭 �ʱ�ȭ
	}

	//--- 2. �޸� �Ҵ�
	int vertIndex = 0;
	int faceIndex = 0;
	rewind(objFile);
	Read.vertex = (vec3*)malloc(sizeof(vec3) * Read.vertexNum);
	Read.face = (FACE*)malloc(sizeof(FACE) * Read.faceNum);
	//--- 3. �Ҵ�� �޸𸮿� �� ���ؽ�, ���̽� ���� �Է�
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&Read.vertex[vertIndex].x, &Read.vertex[vertIndex].y,
				&Read.vertex[vertIndex].z);
			vertIndex++;
		}
		else if (count[0] == 'f' && count[1] == '\0') {
			fscanf(objFile, "%d %d %d",
				&Read.face[faceIndex].a, &Read.face[faceIndex].b, &Read.face[faceIndex].c);
			Read.face[faceIndex].a--; Read.face[faceIndex].b--; Read.face[faceIndex].c--;
			faceIndex++;
		}
	}
}