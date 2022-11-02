#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <stdlib.h>
#include <stdio.h>
#include <random>
#include <cassert>

#define arrlen(x) sizeof(x) / sizeof(GLfloat)
#define MaxFrame 100

enum ShapeIndex
{
	S_LINE = 0,
	S_TRIANGLE,
	S_RECT,
	S_PENTAGON
};

//---윈도우 사이즈 변수
int WinSize_r = 1200;
int WinSize_w = 800;
GLfloat changeratio = (GLfloat)WinSize_r / WinSize_w;
using namespace std;
int windowID;		//---윈도우 아이디

//---콜백 함수
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid TimerFunction(int value);
GLvoid Keyboard(unsigned char key, int x, int y);

//---glsl
void make_vertexShader();
void make_fragmentShader();
void InitShader();
void InitBuffer();
char* filetobuf(const char* file);
GLchar* vertexsource, * fragmentsource;		//---소스코드 저장 변수
GLuint vertexshader, fragmentshader;		//---세이더 객체
GLuint vao_line, vao[4];
GLuint vbo_line, vbo[4];
GLuint ebo[3];
//---쉐이더 프로그램
GLuint s_program;

//---선
GLfloat back_line[12] = { -1.0 , 0, 0,
						1.0, 0, 0, 
						0, 1.0, 0, 
						0, -1.0 };
GLfloat line[9] = { -0.8, 0.2, 0,
					-0.2, 0.8, 0 ,
					-0.5, 0.5, 0};
GLfloat line_finalshape[9] = { -0.8, 0.2, 0,
							-0.2, 0.2, 0,
							-0.5, 0.8, 0 };
GLfloat triangle[12] = { 0.2, 0.2, 0,
						0.8, 0.2, 0,
						0.5, 0.8, 0,
						0.5, 0.8, 0 };
GLfloat triangle_finalshape[12] = { 0.2, 0.25, 0,
								0.8, 0.25, 0,
								0.8, 0.8, 0,
								0.2, 0.8, 0 };
int triangle_ebo[6] = { 0, 1, 2,
					0, 2, 3 };
GLfloat rect[15] = { -0.8, -0.2, 0,
					-0.2, -0.2, 0,
					-0.2, -0.8, 0,
					-0.8, -0.8, 0,
					-0.5, -0.2, 0};
GLfloat rect_finalshape[15] = { -0.75, -0.5, 0,
							-0.25, -0.5, 0,
							-0.33, -0.85, 0,
							-0.67, -0.85, 0,
							-0.5, -0.15, 0 };
int rect_ebo[9] = { 0, 1, 2,
					0, 2, 3,
					0, 1, 4};
GLfloat pentagon[15] = { 0.5, -0.2, 0,
						0.8, -0.5, 0,
						0.65, -0.85, 0,
						0.35, -0.85, 0,
						0.2, -0.5, 0 };
GLfloat pentagon_finalshape[15] = { 0.5, -0.5, 0,
						0.5, -0.5, 0,
						0.5, -0.5, 0,
						0.5, -0.5, 0,
						0.5, -0.5, 0 };
int pentagon_ebo[9] = { 0,1,2,
						0,2,3,
						0,3,4 };

int frame[4] = { 0 };
int shapeloop = 1;
//---추가구현
bool second_anim = false;


GLfloat* ShapeAnim(int index)
{
	int arrlenth = 0;
	switch (index)
	{
	case S_LINE:
		arrlenth = arrlen(line);
		break;
	case S_TRIANGLE:
		arrlenth = arrlen(triangle);

		break;
	case S_RECT:
		arrlenth = arrlen(rect);

		break;
	case S_PENTAGON:
		arrlenth = arrlen(pentagon);

		break;
	}
	if (arrlenth != 0)
	{
		GLfloat* arr = new GLfloat[arrlenth]{ 0 };
		switch (index)
		{
		case S_LINE:
			for (int i = 0; i < arrlenth; i++)
				arr[i] = line[i] + (line_finalshape[i] - line[i]) * ((GLfloat)frame[S_LINE] / MaxFrame);

			frame[S_LINE] = (frame[S_LINE] + shapeloop) % MaxFrame;

			break;
		case S_TRIANGLE:
			for (int i = 0; i < arrlenth; i++)
				arr[i] = triangle[i] + (triangle_finalshape[i] - triangle[i]) * ((GLfloat)frame[S_TRIANGLE] / MaxFrame);
			frame[S_TRIANGLE] = (frame[S_TRIANGLE] + shapeloop) % MaxFrame;

			break;
		case S_RECT:
			for (int i = 0; i < arrlenth; i++)
				arr[i] = rect[i] + (rect_finalshape[i] - rect[i]) * ((GLfloat)frame[S_RECT] / MaxFrame);
			frame[S_RECT] = (frame[S_RECT] + shapeloop) % MaxFrame;

			break;
		case S_PENTAGON:
			for (int i = 0; i < arrlenth; i++)
				arr[i] = pentagon[i] + (pentagon_finalshape[i] - pentagon[i]) * ((GLfloat)frame[S_PENTAGON] / MaxFrame);
			frame[S_PENTAGON] = (frame[S_PENTAGON] + shapeloop) % MaxFrame;
			break;
		}
		if (arr != NULL)
			return arr;
		else
			return 0;
	}
	else
		return 0;
}
//------------추가구현
int add_frame = 0;
int add_Maxframe = 400;
GLfloat add_shape[15] = { -0.5, -0.5, 0,
						0.5, 0.5, 0,
						0,0,0,
						0,0,0,
						0,0,0 };
GLfloat add_shape2[15] = { -0.5, -0.5, 0,
							0.5, -0.5, 0,
							0,0.6,0,
							0,0.6,0,
							0,0.6,0 };
GLfloat add_shape3[15] = { -0.5, -0.5, 0,
							0.5, -0.5, 0,
							-0.5, 0.5, 0,
							0.5, 0.5, 0,
							0.5, 0.5, 0 };
GLfloat add_shape4[15] = { -0.25, -0.6, 0,
							0.25, -0.6, 0,
							-0.4,0.2,0,
							0.4,0.2,0,
							0,0.8,0 };
GLfloat add_shape5[15] = { 0, 0, 0,
							0, 0, 0,
							0, 0, 0,
							0, 0, 0,
							0, 0, 0 };
int add_shape_ebo[9] = { 0,2,4,
					0,1,4,
					1,3,4 };
GLuint add_vao, add_vbo, add_ebo;

GLfloat* additional_ShapeAnim()
{
	GLfloat* arr = new GLfloat[15]{ 0 };
	switch (add_frame / 100)
	{
	case 0:
		for (int i = 0; i < 15; i++)
			arr[i] = add_shape[i] + (add_shape2[i] - add_shape[i]) * ((GLfloat)((add_frame % 100)) / 100);


		break;
	case 1:
		for (int i = 0; i < 15; i++)
			arr[i] = add_shape2[i] + (add_shape3[i] - add_shape2[i]) * ((GLfloat)(add_frame % 100) / 100);

		break;
	case 2:
		for (int i = 0; i < 15; i++)
			arr[i] = add_shape3[i] + (add_shape4[i] - add_shape3[i]) * ((GLfloat)(add_frame % 100) / 100);

		break;
	case 3:
		for (int i = 0; i < 15; i++)
			arr[i] = add_shape4[i] + (add_shape5[i] - add_shape4[i]) * ((GLfloat)(add_frame % 100) / 100);
		break;
	}
	add_frame = (add_frame + 1) % add_Maxframe;

	if (arr != NULL)
		return arr;
	else
		return 0;
}

void main(int argc, char** argv)		//---윈도우 출력, 콜백함수 설정
{
	//---윈도우 생성
	glutInit(&argc, argv);		//glut초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	//디스플레이 모드 설정
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
	
	
	InitShader();
	InitBuffer();

	glutDisplayFunc(drawScene);
	glutReshapeFunc(Reshape);
	glutKeyboardFunc(Keyboard);

	glutTimerFunc(50, TimerFunction, 1);


	glutMainLoop();
}

GLvoid drawScene()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	
	int vColorLocation = glGetUniformLocation(s_program, "vColor");

	glUseProgram(s_program);
	if (second_anim == false)
	{
		//-------------선 그리기
		glUniform4f(vColorLocation, 0.0f, 0.0f, 0.0f, 1.0f);


		glBindVertexArray(vao_line);
		glEnableVertexAttribArray(0);

		glDrawArrays(GL_LINES, 0, 2);
		glDrawArrays(GL_LINES, 2, 2);
		//-------------도형 그리기
		for (int i = 0; i < 4; i++)
		{
			glBindVertexArray(vao[i]);
			glEnableVertexAttribArray(0);
			switch (i)
			{
			case 0:
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
				glUniform4f(vColorLocation, 0.0f, 0.0f, 1.0f, 1.0f);
				glDrawArrays(GL_TRIANGLES, 0, 3);
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				break;
			case 1:
				glUniform4f(vColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
				break;
			case 2:
				glUniform4f(vColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);
				glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
				break;
			case 3:
				glUniform4f(vColorLocation, 0.2f, 0.5f, 0.5f, 1.0f);
				glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
				break;
			}
		}

	}
	else
	{
		glBindVertexArray(add_vao);
		glEnableVertexAttribArray(0);
		
		if(add_frame == 0)	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		switch (add_frame / 100)
		{
		case 0:
			glUniform4f(vColorLocation, 0.1f, 0.5f, 0.1f, 1.0f);
			break;
		case 1:
			glUniform4f(vColorLocation, 1.0f, 0.0f, 0.0f, 1.0f);
			break;
		case 2:
			glUniform4f(vColorLocation, 0.0f, 1.0f, 0.2f, 1.0f);
			break;
		case 3:
			glUniform4f(vColorLocation, 0.2f, 0.5f, 0.4f, 1.0f);
			break;
		}
		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
		if (add_frame == 0)	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	glBindVertexArray(0);
	glDisableVertexAttribArray(0);
	glutSwapBuffers();
}

GLvoid Reshape(int w, int h)
{
	glViewport(0, 0, w, h);

}

void make_vertexShader()
{
	vertexsource = filetobuf("vertex_2d.glsl");

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

void InitShader()
{
	make_vertexShader(); //--- 버텍스 세이더 만들기
	make_fragmentShader(); //--- 프래그먼트 세이더 만들기
	//-- shader Program
	s_program = glCreateProgram();

	glAttachShader(s_program, vertexshader);
	glAttachShader(s_program, fragmentshader);

	//--- 세이더 삭제하기
	glDeleteShader(vertexshader);
	glDeleteShader(fragmentshader);

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
	glGenVertexArrays(4, vao);
	glGenBuffers(4, vbo);
	glGenBuffers(3, ebo);

	for (int i = 0; i < 4; i++)
	{
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);

		switch (i)
		{
		case 0:
			glBufferData(GL_ARRAY_BUFFER, arrlen(line) * sizeof(GLfloat), line, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			break;
		case 1:
			glBufferData(GL_ARRAY_BUFFER, arrlen(triangle) * sizeof(GLfloat), triangle, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[0]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(triangle_ebo) * sizeof(int), triangle_ebo, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			break;
		case 2:
			glBufferData(GL_ARRAY_BUFFER, arrlen(rect) * sizeof(GLfloat), rect, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[1]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(rect_ebo) * sizeof(int), rect_ebo, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			break;
		case 3:
			glBufferData(GL_ARRAY_BUFFER, arrlen(pentagon) * sizeof(GLfloat), pentagon, GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo[2]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, arrlen(pentagon_ebo) * sizeof(int), pentagon_ebo, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			break;
		}

	}
	//----------------추가구현
	glGenVertexArrays(1, &add_vao);
	glGenBuffers(1, &add_vbo);
	glGenBuffers(1, &add_ebo);

	glBindVertexArray(add_vao);

	glBindBuffer(GL_ARRAY_BUFFER, add_vbo);

	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), add_shape, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, add_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 9 * sizeof(int), add_shape_ebo, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);



	glEnableVertexAttribArray(0);
}

void changebuffer()
{
	for (int i = 0; i < 4; i++)
	{
		glBindVertexArray(vao[i]);
		glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);

		GLfloat* arr = NULL;
		switch (i)
		{
		case 0:
			arr = ShapeAnim(S_LINE);

			glBufferData(GL_ARRAY_BUFFER, arrlen(line) * sizeof(GLfloat), arr, GL_STATIC_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
			break;
		case 1:
			arr = ShapeAnim(S_TRIANGLE);

			glBufferData(GL_ARRAY_BUFFER, arrlen(triangle) * sizeof(GLfloat), arr, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			break;
		case 2:
			arr = ShapeAnim(S_RECT);

			glBufferData(GL_ARRAY_BUFFER, arrlen(rect) * sizeof(GLfloat), arr, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			break;
		case 3:
			arr = ShapeAnim(S_PENTAGON);

			glBufferData(GL_ARRAY_BUFFER, arrlen(pentagon) * sizeof(GLfloat), arr, GL_STATIC_DRAW);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
			break;
		}
		if(arr !=NULL) delete arr;
	}
	glEnableVertexAttribArray(0);
}
void change_add_buffer()
{
	GLfloat* arr = NULL;
	arr = additional_ShapeAnim();

	glBindVertexArray(add_vao);

	glBindBuffer(GL_ARRAY_BUFFER, add_vbo);

	glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), arr, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	if (arr != NULL) delete arr;
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
GLvoid Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		glutDestroyWindow(windowID);
		break;
	case 'a':
		if (second_anim == false)
		{
			add_frame = 0;
			second_anim = true;
			glutTimerFunc(10, TimerFunction, 2);
		}
		else
		{
			shapeloop = 1;
			for (int i = 0; i < 4; i++)
			{
				frame[i] = 0;
			}
			second_anim = false;
			glutTimerFunc(10, TimerFunction, 1);

		}
		break;
	}
	glutPostRedisplay();
}
GLvoid TimerFunction(int value)
{
	if (second_anim == false && value == 1)
	{
		changebuffer();
		if (frame[0] >= MaxFrame - 1 || frame[0] < 0)
		{
			shapeloop *= -1;
			glutTimerFunc(300, TimerFunction, 1);
		}
		else
			glutTimerFunc(10, TimerFunction, 1);
	}
	else if(second_anim == true && value == 2)
	{
		change_add_buffer();

		glutTimerFunc(10, TimerFunction, 2);
	}
	glutPostRedisplay();
}