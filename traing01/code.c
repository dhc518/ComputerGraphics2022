 화면에 800 x 600 크기의 윈도우를 띄운다.
 초기 배경색은 흰색
GLvoid drawScene ( ) //--- 콜백 함수: 그리기 콜백 함수 
{
glClearColor( 1.0f, 1.0f, 1.0f, 1.0f ); // 바탕색을 ‘blue’ 로 지정
glClear( GL_COLOR_BUFFER_BIT ); // 설정된 색으로 전체를 칠하기
// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
glutSwapBuffers (); // 화면에 출력하기
}

 윈도우를 띄우고 배경색을 키보드 입력에 따라 다양하게 적용 해 보기
 키보드 입력 값:
 R: 빨간색
 G: 초록색
 B: 파란색
 A: 렌덤색
 W: 백색
 K: 검정색
 T: 타이머를 설정하여 특정 시간마다 렌덤색으로 계속 바뀌게 한다.
 S: 타이머 종료
 Q: 프로그램 종료

GLvoid Keyboard ( unsigned char key, int x, int y)
{
        switch (key) {
                case ‘R’: …; break; //--- 배경색을 빨강색으로 설정
                case ‘G’: …; break; //--- 배경색을 초록색으로 설정
                case ‘B’: …; break; //--- 배경색을 파랑색으로 설정

        }
        glutPostRedisplay (); //--- 배경색이 바뀔 때마다 출력 콜백 함수를 호출하여 화면을 refresh 한다
}
