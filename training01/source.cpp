#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include<stdlib.h>
#include<stdlib.h>
#include<time.h>

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);

double r = 1.0, g = 1.0, b = 1.0;
int onoff;



void main(int argc, char** argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(800, 600);
    glutCreateWindow("trainig1");

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Unable to initialize GLEW" << std::endl;
        exit(EXIT_FAILURE);
    }
    else std::cout << "GLEW Initialized\n";

    glutDisplayFunc(drawScene);
    glutReshapeFunc(Reshape);
    glutKeyboardFunc(Keyboard);
    glutMainLoop();
}

void random_color_timer(int value)
{
    if (onoff == 1) {
        Keyboard('a', 0, 0);
        glutTimerFunc(1000, random_color_timer, 1); // 타이머함수 재 설정
    } 
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
    glClearColor(r, g, b, 1.0f); // 바탕색 지정
    glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
    // 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
    glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
    glViewport(0, 0, w, h);
}

GLvoid Keyboard(unsigned char key, int x, int y)
{
    srand((unsigned int)time(NULL));
    switch (key) {
    case 'r':
        r = 1.0, g = 0.0, b = 0.0;
        break; //--- 배경색을 빨강색으로 설정

    case 'g':
        r = 0.0, g = 1.0, b = 0.0;
        break; //--- 배경색을 초록색으로 설정

    case 'b':
        r = 0.0, g = 0.0, b = 1.0;
        break; //--- 배경색을 파랑색으로 설정
    case 'a':
        r = (rand()%101)/100.0, g = (rand() % 100) / 100.0, b = (rand() % 100) / 100.0;
        break;//랜덤값
    case 'w':
        r = 1.0, g = 1.0, b = 1.0;
        break;//백
    case 'k':
        r = 0.0, g = 0.0, b = 0.0;
        break;//흑
    case 't':
        onoff = 1;
        glutTimerFunc(1000, random_color_timer, 1);
        break;//타이머 설정 랜덤
    case 's':
        onoff = 0;
        break;//타이머 종료
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}