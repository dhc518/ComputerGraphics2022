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

#define initialRotate_x -30
#define initialRotate_y 30

//---윈도우 사이즈 변수
int WinSize_r = 1000;
int WinSize_w = 1000;
GLfloat changeratio = (GLfloat)WinSize_r / WinSize_w;
using namespace std;
int windowID;		//---윈도우 아이디

//---콜백 함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void SpecialKeyboard(int key, int x, int y);
GLvoid TimerFunction(int value);

//---glsl
void make_vertexShader();
void make_fragmentShader();
void InitShader();
void InitBuffer();
char* filetobuf(const char* file);
void ReadObj(FILE* objFile);
GLchar* vertexsource[2], * fragmentsource[2];		//---소스코드 저장 변수
GLuint vertexshader[2], fragmentshader[2];		//---세이더 객체
GLuint vao_line, vao;
GLuint vbo_line, vbo[2];
GLuint ebo;
//---쉐이더 프로그램
GLuint s_program;
GLuint s_program_line;

//---선
GLfloat back_line[12] = { -1.0 , 0, 0,
						1.0, 0, 0, 
						0, 1.0, 0, 
						0, -1.0 };

glm::vec3* vertex = NULL;
int* face = NULL;



GLfloat cubeColor[24];
GLfloat pyramidColor[18];

bool depthTest = false;

bool IsCube = true;

GLfloat rotate_x = initialRotate_x, rotate_y = initialRotate_y;
GLfloat trans_x = 0, trans_y = 0;

int RotateAnim_x = 0, RotateAnim_y = 0;
void RandRGB()
{
	std::random_device rd;
	std::default_random_engine eng(rd());
	std::uniform_real_distribution<GLclampf> rd_RGB(0.0, 1.0f);

	for (int i = 0; i < 24; i++)
	{
		cubeColor[i] = rd_RGB(eng);
	}
	for (int i = 0; i < 18; i++)
		pyramidColor[i] = rd_RGB(eng);
}

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

	InitShader();
	InitBuffer();



	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKeyboard);

	glutMainLoop();
}
GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	//-------------선 그리기
	int vColorLocation_line = glGetUniformLocation(s_program_line, "vColor");

	glUseProgram(s_program_line);

	glBindVertexArray(vao_line);
	glEnableVertexAttribArray(0);
	glUniform4f(vColorLocation_line, 0.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINES, 0, 2);
	glDrawArrays(GL_LINES, 2, 2);

	//----------도형 그리기
	

	int vColorLocation = glGetUniformLocation(s_program, "vColor");
	glUseProgram(s_program);

	glm::mat4 TR = glm::mat4(1.0f);
	TR = glm::translate(TR, glm::vec3(trans_x, trans_y, 0.0)); 
	TR = glm::rotate(TR, (GLfloat)glm::radians(rotate_x), glm::vec3(1.0, 0.0, 0.0));
	TR = glm::rotate(TR, (GLfloat)glm::radians(rotate_y), glm::vec3(0.0, 1.0, 0.0));

	unsigned int transformLocation = glGetUniformLocation(s_program, "transform");
	glUniformMatrix4fv(transformLocation, 1, GL_FALSE, glm::value_ptr(TR));

	glBindVertexArray(vao);
		

	if (IsCube == true)
	{
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0));

	}
	else
	{
		glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, (GLvoid*)(sizeof(GLuint) * 0));

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
	vertexsource[0] = filetobuf("vertex13.glsl");
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
	fragmentsource[0] = filetobuf("fragment13.glsl");
	fragmentsource[1] = filetobuf("fragment.glsl");

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
	//--------------선 그리기
	glGenVertexArrays(1, &vao_line);
	glGenBuffers(1, &vbo_line);
	glBindVertexArray(vao_line);

	glBindBuffer(GL_ARRAY_BUFFER, vbo_line);

	glBufferData(GL_ARRAY_BUFFER,  12 * sizeof(GLfloat), back_line, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(0);




	//----------------도형
	FILE* fp;
	fp = fopen("cube_13.obj", "rb");
	ReadObj(fp);

	glGenVertexArrays(1,& vao);
	glGenBuffers(2, vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), face, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cubeColor, GL_STATIC_DRAW);


	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
}

void chageShape()
{
	glBindVertexArray(vao);
	FILE* fp;
	//----------------도형
	if (IsCube == true)
	{
		fp = fopen("cube_13.obj", "rb");
		ReadObj(fp);


		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), vertex, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(int), face, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 24 * sizeof(GLfloat), cubeColor, GL_STATIC_DRAW);


		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
	}
	else
	{
		fp = fopen("pyramid_13.obj", "rb");
		ReadObj(fp);


		glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
		glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertex, GL_STATIC_DRAW);


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 18 * sizeof(int), face, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
		glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), pyramidColor, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(1);
	}
}
GLvoid TimerFunction(int value)
{
	if (value == 1 && RotateAnim_x != 0)
	{
		rotate_x += RotateAnim_x * 1;
		glutTimerFunc(10, TimerFunction, 1);
	}
	else if (value = 2 && RotateAnim_y != 0)
	{
		rotate_y += RotateAnim_y * 1;
		glutTimerFunc(10, TimerFunction, 2);
	}
	
	glutPostRedisplay();
}
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'c':
		IsCube = true;
		break;
	case 'p':
		IsCube = false;
		break;
	case 'h':
		if (depthTest == false)
		{
			glEnable(GL_DEPTH_TEST);
			depthTest = true;
		}
		else
		{
			glDisable(GL_DEPTH_TEST);
			depthTest = false;
		}
		break;
	case 'x':
		if (RotateAnim_x != 1)
		{
			if (RotateAnim_x == 0)glutTimerFunc(10, TimerFunction, 1);

			RotateAnim_x = 1;
		}
		else RotateAnim_x = 0;
		break;
	case 'X':
		if (RotateAnim_x != -1)
		{
			if(RotateAnim_x == 0)glutTimerFunc(10, TimerFunction, 1);

			RotateAnim_x = -1;
		}
		else RotateAnim_x = 0;
		break;
	case 'y':
		if (RotateAnim_y != 1)
		{
			if (RotateAnim_y == 0)glutTimerFunc(10, TimerFunction, 2);

			RotateAnim_y = 1;
		}
		else RotateAnim_y = 0;
		break;
	case 'Y':
		if (RotateAnim_y != -1)
		{
			if (RotateAnim_y == 0)glutTimerFunc(10, TimerFunction, 2);

			RotateAnim_y = -1;
		}
		else RotateAnim_y = 0;
		break;
	case 'w':
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		break;
	case 'W':
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		break;
	case 's':
		RotateAnim_x = 0; RotateAnim_y = 0;
		rotate_x = initialRotate_x; rotate_y = initialRotate_y;
		trans_x = 0; trans_y = 0;
		break;
	case 'q':
		glutDestroyWindow(windowID);
		break;
	}
	chageShape();
	glutPostRedisplay();
}

void SpecialKeyboard(int key, int x, int y)
{
	switch (key)
	{

	case GLUT_KEY_LEFT:
		if (trans_x > -0.9)trans_x -= 0.05;
		break;
	case GLUT_KEY_RIGHT:
		if (trans_x < 0.9)trans_x += 0.05;
		break;
	case GLUT_KEY_UP:
		if (trans_y < 0.9)trans_y += 0.05;
		break;
	case GLUT_KEY_DOWN:
		if (trans_y > -0.9)trans_y -= 0.05;
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

void ReadObj(FILE* objFile)
{
	//--- 1. 전체 버텍스 개수 및 삼각형 개수 세기
	char count[100];
	int vertexNum = 0;
	int faceNum = 0;
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0')
			vertexNum += 1;
		else if (count[0] == 'f' && count[1] == '\0')
			faceNum += 1;
		memset(count, '\0', sizeof(count)); // 배열 초기화
	}
	//--- 2. 메모리 할당

	int vertIndex = 0;
	int faceIndex = 0;
	if (vertex != NULL) free(vertex);
	if (face != NULL) free(face);

	vertex = (glm::vec3*)malloc(sizeof(glm::vec3) * vertexNum);
	face = (int*)malloc(sizeof(int) * faceNum * 3);

	rewind(objFile);

	//--- 3. 할당된 메모리에 각 버텍스, 페이스 정보 입력
	while (!feof(objFile)) {
		fscanf(objFile, "%s", count);
		if (count[0] == 'v' && count[1] == '\0') {
			fscanf(objFile, "%f %f %f",
				&vertex[vertIndex].x, &vertex[vertIndex].y,
				&vertex[vertIndex].z);
			vertIndex++;
		}
		else if (count[0] == 'f' && count[1] == '\0') {
			fscanf(objFile, "%d %d %d",
				&face[faceIndex * 3], &face[faceIndex * 3 + 1], &face[faceIndex * 3 + 2]);
			faceIndex++;
		}
	}
}