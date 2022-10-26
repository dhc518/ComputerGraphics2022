#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
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

#define PI 3.141592

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

typedef struct CYCLONE {
	float point[300][3];
	float radius;
	float origin[2], theta;
	int point_count;
	float radian;
};

//����
GLchar* vertexsource[2], * fragmentsource[2];
GLuint vertexshader[2], fragmentshader[2], vao[3], vbo[3], ebo[1];
GLuint s_program, s_program_plat, shaderID;
GLfloat rColor = 1.f, bColor = 1.f, gColor = 1.f;
GLint result;
GLchar errorLog[512];

GLUquadricObj* qobj;

GLfloat middle_line[6][3] = {};

READ cube;
READ tetra;

CYCLONE cyclone;

bool isOrigin = true;

int a = 0;
bool plus_a = true;

int s_count = 0;
bool plus_s = true;

bool isR_on = false;
bool isT_on = false;
bool isS_on = false;

float x_trans = 0;
float y_trans = 0;

float one_x = 0;
float one_y = 0;
float one_z = 0;
float one_scale = 1;
float one_zero_scale = 1;


float two_x = 0;
float two_y = 0;
float two_z = 0;
float two_scale = 1;
float two_zero_scale = 1;



mat4 TR_CYC2 = mat4(1.f);
mat4 S_TR = mat4(1.f);


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
void draw_cyclone();
void enter();

void Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
void Timer(int value);


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
	enter();
	draw_cyclone();

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
	int vColorLocation2 = glGetUniformLocation(s_program, "out_Color");
	//--- ����� ���� ����
	glClearColor(rColor, gColor, bColor, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- ������ ���������ο� ���̴� �ҷ�����
	glUseProgram(s_program);

	//--- ����� VAO �ҷ�����
	glBindVertexArray(vao[0]);
	glEnableVertexAttribArray(0);

	mat4 xyz = mat4(1.0f);
	xyz = rotate(xyz, radians(30.f), vec3(1.f, 0, 0));
	xyz = rotate(xyz, radians(30.f), vec3(0, -1.f, 0));

	int xyzLocation = glGetUniformLocation(s_program, "modelTransform");
	//--- modelTransform ������ ��ȯ �� �����ϱ�
	glUniformMatrix4fv(xyzLocation, 1, GL_FALSE, value_ptr(xyz));

	//�߾Ӽ�
	glUniform3f(vColorLocation2, 0, 0, 0);
	glDrawArrays(GL_LINES, 0, 6);

	//ȸ����
	glBindVertexArray(vao[2]);
	glEnableVertexAttribArray(0);

	cout << cyclone.point_count << endl;
	glPointSize(5);
	glDrawArrays(GL_LINE_STRIP, 0, cyclone.point_count - 18);

	//���� �׸���
	glEnable(GL_DEPTH_TEST);

	//glUseProgram(s_program);
	int vColorLocation = glGetUniformLocation(s_program, "out_Color");

	mat4 model = mat4(1.0f);  mat4 cube_model = mat4(1.f);
	mat4 TR_CYC = mat4(1.f);  mat4 TR_CYC2 = mat4(1.f);
	mat4 S_TR = mat4(1.f); mat4 S_TR2 = mat4(1.f);

	model = scale(model, vec3(one_zero_scale, one_zero_scale, one_zero_scale));
	model = rotate(model, radians(30.f), vec3(1.f, 0, 0));
	model = rotate(model, radians(30.f), vec3(0, -1.f, 0));
	model = translate(model, vec3(0.5f, 0, 0));
	model = translate(model, vec3(x_trans, 0, y_trans));
	model = translate(model, vec3(one_x, one_y, one_z));
	model = scale(model, vec3(one_scale, one_scale, one_scale));


	if (isR_on) {
		model = translate(model, vec3(-0.5f, 0, 0));
		TR_CYC = translate(TR_CYC, vec3(cyclone.point[a][0], 0, cyclone.point[a][2]));
	}

	if (isS_on || isT_on) {
		if (plus_s) {
			S_TR = translate(S_TR, vec3(s_count * -0.05f, 0, 0));
			s_count++;
		}
		else if (!plus_s) {
			S_TR = translate(S_TR, vec3(s_count * -0.05f, 0, y_trans / 10));
			s_count--;
		}
	}

	cout << s_count << endl;


	unsigned int modelLocation = glGetUniformLocation(s_program, "modelTransform");
	//--- modelTransform ������ ��ȯ �� �����ϱ�
	//glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(RT2_1 * TR * RT * RT1 * model));
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model * TR_CYC * S_TR));


	glUniform3f(vColorLocation, 0, 0, 0);

	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);// ���� ��Ÿ��
	gluQuadricNormals(qobj, GLU_SMOOTH); //���� ����
	gluQuadricOrientation(qobj, GLU_OUTSIDE);// ���� ����
	gluSphere(qobj, 0.1, 30, 30);


	//ť��
	cube_model = scale(cube_model, vec3(two_zero_scale, two_zero_scale, two_zero_scale));
	cube_model = rotate(cube_model, radians(30.f), vec3(1.f, 0, 0));
	cube_model = rotate(cube_model, radians(30.f), vec3(0, -1.f, 0));
	cube_model = translate(cube_model, vec3(-0.5f, 0, 0));
	cube_model = translate(cube_model, vec3(x_trans, 0, y_trans));
	cube_model = translate(cube_model, vec3(two_x, two_y, two_z));
	cube_model = scale(cube_model, vec3(two_scale, two_scale, two_scale));

	if (isR_on) {
		cube_model = translate(cube_model, vec3(0.5f, 0, 0));
		TR_CYC2 = translate(TR_CYC2, vec3(cyclone.point[cyclone.point_count - 19 - a][0], 0, cyclone.point[cyclone.point_count - 19 - a][2]));
	}

	if (isS_on || isT_on) {
		if (plus_s) {
			S_TR2 = translate(S_TR2, vec3(s_count * 0.05f, 0, 0));
			s_count++;
		} else if (!plus_s) {
			S_TR2 = translate(S_TR2, vec3(s_count * 0.05f, 0, 0));
			s_count--;
		}
	}

	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(cube_model * TR_CYC2 * S_TR2));


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
	glGenBuffers(1, ebo);

	//--- attribute �ε��� 0���� ��밡���ϰ� ��
	glBindVertexArray(vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);

	glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(GLfloat), middle_line, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//ȸ����
	glBindVertexArray(vao[2]);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, cyclone.point_count * 3 * sizeof(GLfloat), cyclone.point, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	//������ü
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
	case 'p':
		x_trans = 0;
		y_trans = 0;
		glutPostRedisplay();
		break;
	case 'r':
		isR_on = !isR_on;
		glutTimerFunc(10, Timer, 1);
		break;

	case 's':
		isT_on = !isT_on;
		glutTimerFunc(100, Timer, 1);
		break;

	case 't':
		isS_on = !isS_on;
		glutTimerFunc(100, Timer, 1);
		break;
	case 'x':	
		one_x += 0.01f;
		break;

	case 'y':	
		one_y += 0.01f;
		break;

	case 'z':	
		one_z += 0.01f;
		break;

	case 'X':
		two_x += 0.01f;
		break;

	case 'Y':
		two_y += 0.01f;
		break;

	case 'Z':
		two_z += 0.01f;
		break;

	case 'm':
		one_scale += 0.01f;
		break;

	case 'n':
		one_scale -= 0.01f;
		break;

	case 'M':
		two_scale += 0.01f;
		break;

	case 'N':
		two_scale -= 0.01f;
		break;

	case 'k':
		one_zero_scale += 0.01f;
		break;

	case 'j':
		one_zero_scale -= 0.01f;
		break;

	case 'K':
		two_zero_scale += 0.01f;
		break;

	case 'J':		
		two_zero_scale += 0.01f;
		break;


	case 'q':
		glutLeaveMainLoop();
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
	if (isR_on) {
		if (plus_a) {
			a++;
			if (a == cyclone.point_count - 19)
				plus_a = !plus_a;
		}

		else if (!plus_a) {
			a--;
			if (a == 0)
				plus_a = !plus_a;
		}
			

	}

	if (isS_on) {
		if (plus_s) {
			if (s_count == 10) {
				plus_s = !plus_s;
			}
		}

		else if (!plus_s) {
			if (s_count == 0) {
				plus_s = !plus_s;
			}
		}
	}

	if (isT_on) {
		if (plus_s) {
			if (s_count == 20) {
				plus_s = !plus_s;
			}
		}

		else if (!plus_s) {
			if (s_count == 0) {
				plus_s = !plus_s;
			}
		}
	}

	if (!isR_on && !isT_on && !isS_on) {
		a = 0;
		plus_a = true;
		plus_s = true;
		s_count = 0;
		return;
	}

	glutTimerFunc(30, Timer, 1);

	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_DOWN) {
		y_trans += 0.01f;
	}

	else if (key == GLUT_KEY_UP) {
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

void draw_middle_line()
{
	middle_line[0][0] = 1.f;  middle_line[0][1] = 0;  middle_line[0][2] = 0;
	middle_line[1][0] = -1.f;  middle_line[1][1] = 0;  middle_line[1][2] = 0;
	middle_line[2][0] = 0;  middle_line[2][1] = 1.f;  middle_line[2][2] = 0;
	middle_line[3][0] = 0;  middle_line[3][1] = -1.f;  middle_line[3][2] = 0;
	middle_line[4][0] = 0;  middle_line[4][1] = 0;  middle_line[4][2] = 1.f;
	middle_line[5][0] = 0;  middle_line[5][1] = 0;  middle_line[5][2] = -1.f;
}

void enter()
{
	cyclone.origin[0] = 0;
	cyclone.origin[1] = 0;
	cyclone.theta = 0;
	cyclone.radius = 0.1f;
	cyclone.radian = 0;
	cyclone.point_count = 18;
}

void draw_cyclone() {
	int temp = 0;
	int i = 0;
	for (int j = 0; j < 6; j++) {
		//cyclone.radian = 0;
		for (int i = temp; i < cyclone.point_count; i++) {
			cyclone.point[i][0] = cyclone.origin[0] + cyclone.radius * cos(radians(cyclone.radian)); cyclone.point[i][1] = 0;
			cyclone.point[i][2] = cyclone.radius * sin(radians(cyclone.radian));
			cyclone.radian += 180 /(cyclone.point_count - temp);
			//cyclone.radian %= 360;
			//cyclone.theta = PI * cyclone.radian / 180;
		}
		temp = cyclone.point_count;
		cyclone.point_count += 18;
		cyclone.radius += 0.1f;

		isOrigin = !isOrigin;
		if (isOrigin) {
			cyclone.origin[0] = 0;
		}
		else if (!isOrigin) {
			cyclone.origin[0] = 0.1f;
		}
	}

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