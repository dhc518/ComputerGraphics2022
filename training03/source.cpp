#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include<stdlib.h>
#include<stdlib.h>
#include<time.h>

/*
 사각형 이동하기
     화면 중앙에 사각형을 그린 후,
     마우스 버튼을 사각형 위에 클릭한 채로 드래그 하면
         사각형의 위치가 이동된다.
         마우스를 놓으면 더 이상 사각형이 이동하지 않는다.
 키보드를 눌러 사각형을 추가로 만든다.
     키보드 a: 화면 중앙에 다른 색상의 사각형을 만든다. 최대 5개 만든다. 새롭게 만든 사각형도 이동할 수 있다.
     사각형이 겹쳐져있으면 나중에 만든 사각형이 위에 올라오고 그 사각형이 선택된다
*/

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Keyboard(unsigned char key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);

struct rect {
    GLfloat size;
    GLfloat r, g, b;
};

GLfloat r = 1.0, g = 1.0, b = 1.0;
int rect_num;//사각형 숫자
int select_rect//선택된 사각형

rect moving_rect[5];//움직이는 사각형 구조체 선언
//사각형 램덤 색상 r = (rand()%101)/100.0, g = (rand() % 100) / 100.0, b = (rand() % 100) / 100.0;


void main(int argc, char** argv)
{
    moving_rect[0].size = 0.9;
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
    glutMotionFunc(Motion);
    glutMainLoop();
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
    glClearColor(r, g, b, 1.0f); // 바탕색 지정
    glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기
    // 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
    for (rect_num = 0; rect_num < 5; rect_num++) {
        glColor3f(moving_rect[rect_num].r, moving_rect[rect_num].g, moving_rect[rect_num].b);
        if(moving_rect[rect_num].size != NULL)
        glRectf(-1 * moving_rect[rect_num].size, -1 * moving_rect[rect_num].size, moving_rect[rect_num].size, moving_rect[rect_num].size);
    }
    


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
        //Create_rect();
        break;//랜덤값
    case 'q':
        exit(0);
        break;
    }
    glutPostRedisplay(); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}

void Create_rect() {

}

void Mouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {        
        left_button = true;
    }

    glutPostRedisplay();
}

void Motion(int x, int y) {
    if (left_button == true) {
       
    }

    glutPostRedisplay();
}

void Find_rectangle(int x, int y) {
    

}


