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
void Mouse(int button, int state, int x, int y);

struct rect {
    GLfloat size;
    GLfloat r, g, b;
};

GLfloat r = 1.0, g = 1.0, b = 1.0;
int onoff;
rect rect1;



void main(int argc, char** argv)
{
    rect1.size = 0.5;
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
    glutMouseFunc(Mouse);
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
    glColor3f(rect1.r, rect1.g, rect1.b);
    glRectf(-1*rect1.size, -1 * rect1.size, rect1.size, rect1.size);

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
   
    case 'a':
        r = (rand()%101)/100.0, g = (rand() % 100) / 100.0, b = (rand() % 100) / 100.0;
        break;//랜덤값
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
        if (600 + 400 * (rect1.size - 0.5) >= x && x >= 200 - 400 * (rect1.size - 0.5) && 450 + 300 * (rect1.size - 0.5) >= y && y >= 150 - 300 * (rect1.size - 0.5)) {
            rect1.r = (rand() % 101) / 100.0, rect1.g = (rand() % 100) / 100.0, rect1.b = (rand() % 100) / 100.0;
        }
        else {
            r = (rand() % 101) / 100.0, g = (rand() % 100) / 100.0, b = (rand() % 100) / 100.0;
        }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
        if (600+400*(rect1.size-0.5)>= x && x >= 200- 400 * (rect1.size - 0.5) && 450+ 300 * (rect1.size - 0.5) >= y && y >= 150- 300 * (rect1.size - 0.5)) {
            rect1.size = rect1.size + 0.1;
        }
        else {
            rect1.size = rect1.size - 0.1;
        }
    glutPostRedisplay();
}

