
#define _CRT_SECURE_NO_WARNINGS //표준 C의 secure 문제 무시하도록 설정

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX 1000
char s[MAX];
char s2[MAX];
char s3[MAX] = "\0";
char delimit[] = " .#@%";
char space[] = " ";
char instead[];
char* token;
char y[MAX];
char w = 'e';
char W = 'E';
char* p;
int x;
int I;
int count_word(char* s);
int k;


int main(void)
{
	/*
	command: d: 문장 전체를 뒤집기
e: 동일한 간격으로 띄어쓰기를 일정 개수 삽입하기
f: 띄어쓰기를 기준으로 문자 뒤집기
g: 문자 내부의 특정 문자열을 다른 문자열로 바꾸기
h: 앞뒤에서 읽었을 때 어디까지 같은 문자인지 출력하기
q: 종료
	*/

	FILE* fp = fopen("data.txt", "r");  //test파일을 r(읽기) 모드로 열기
	char buffer[MAX] = { 0, };

	fread(buffer, 1, MAX, fp); //전체 읽기
	printf("%s", buffer);
	printf("\n");


	while ((x != 'q') && (x != 'Q')) //반복문 시작
	{

		printf("명령어 리스트\n");
		printf("g: 특정 문자를 모두 다른 문자로 바꾸고 출력\n");//명령어를 다시 실행하면 소문자로
		printf("f / F : 모든 공백에 랜덤한 문자(@, %, # 또는 .)를 넣고 출력\n");//명령어를 다시 실행하면 램덤문자를 공백으로
		printf("l / L : 단어의 개수를 출력\n");
		printf("c / C : 대문자는 소문자로, 소문자는 대문자로 바꾸기\n");
		printf("a / A : 문장의 문자를 좌측으로 한 문자씩 이동한다.\n");
		printf("q / Q : 프로그램을 종료\n");
		printf("명령어를 입력하세요: \n\n");
		x = _getch();

		if ((x == 'l') || (x == 'L'))//L조건
		{
			int wc = count_word(s);
			printf("단어의 개수 : % d \n\n", wc);
		}//L끝

		if ((x == 'f') || (x == 'F'))//f조건
		{
			if (0 == strcmp(space, " "))
			{
				srand(time(NULL));
				k = rand() % 3;
				if (k == 0)
				{
					strcpy(space, "@");
				}
				if (k == 1)
				{
					strcpy(space, "%");
				}
				if (k == 2)
				{
					strcpy(space, "#");
				}
				if (k == 3)
				{
					strcpy(space, ".");
				}
			}
			else
				strcpy(space, " ");

			token = strtok(s, delimit); //문장 토큰으로 분리 

			while (token != NULL) //토큰 문장으로 재배치
			{
				strcat(s2, token);
				token = strtok(NULL, delimit);
				if (token == NULL)
				{
					strcat(s2, ".");
				}
				if (token != NULL)
				{
					strcat(s2, space);
				}
			}
			strcpy(s, s2);
			strcpy(s2, s3);
			printf("문장: %s\n\n", s);
		}//f끝

		if ((x == 'G') || (x == 'g'))//g조건
		{

			token = strtok(s, delimit); //문장 토큰으로 분리 

			while (token != NULL) //토큰 문장으로 재배치
			{
				p = strchr(token, w);
				if (p != NULL)
				{
					_strupr(token);
				}
				p = strchr(token, W);
				if (p != NULL)
				{
					_strupr(token);
				}
				strcat(s2, token);

				token = strtok(NULL, delimit);
				if (token == NULL)
				{
					strcat(s2, ".");
				}
				if (token != NULL)
				{
					strcat(s2, space);
				}
			}
			strcpy(s, s2);
			strcpy(s2, s3);//s3으로 s2 초기화
			printf("문장: %s\n\n", s);
		}//g끝

		if ((x == 'c') || (x == 'C'))//c조건
		{

			while (s[I] != 0x0) {  // 아스키 코드 이용
				if (s[I] >= 'A' && s[I] <= 'Z')
				{
					s[I] = s[I] + 32;// 대문자 -> 소문자
				}
				else if (s[I] >= 'a' && s[I] <= 'z')
				{
					s[I] = s[I] - 32;// 소문자 -> 대문자
				}
				I = I + 1;
			}
			I = 0;//I초기화
			printf("문장: %s\n\n", s);
		}//c끝


		if ((x == 'a') || (x == 'A'))//a조건
		{
			s2[0] = s[0];
			while (s[I] != 0x0) {

				s[I] = s[(I + 1)];



				I = I + 1;
			}
			s[(I - 2)] = s2[0];
			strcat(s, ".");
			I = 0;
			strcpy(s2, s3);//s3으로 s2 초기화
			printf("문장: %s\n\n", s);
		}//a끝


	}//반복문 끝
	return 0;
}

