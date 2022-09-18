#define MAX 1000 //특정 문제를 조건부로 컴파일하기 위해 번호 수정 후 컴파일
#define _CRT_SECURE_NO_WARNINGS //표준 C의 secure 문제 무시하도록 설정

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
/*
문자와 숫자가 10줄 저장된 파일을 만든다. (파일 이름: data.txt)
• 데이터 파일 이름을 입력 받고, 파일에서 데이터를 읽어 문자열에 저장한다.
• 읽은 문자열을 출력하고, 공백을 기준으로 단어와 숫자의 개수를 세고, 대문자가 포함된 단어의 개수를
세어 출력한다.
• 1개 이상의 공백이 있는 경우는 1개로 취급한다.
• 알파벳이나 숫자 외의 특수문자들은 (예, / , - * 같은 문자)는 구분하지 않고 연결된 값으로 카운트 한다.
• 숫자와 문자가 연결된 문자인 경우에는 문자로 카운트 하도록 한다.

word count: 32
number count: 13 (1 2 3 4 5 6 7 8 9 10 2022 1001 1001)
Capital word: 12 (This File Fall Tech University Korea
Game Monday Tuesday Wednesday Thursday Friday)
*/

int main() {
    
    int wc, nc, uc;

    FILE* fp = fopen("data.txt", "r");  //test파일을 r(읽기) 모드로 열기
    char buffer[MAX] = {0, };

    fread(buffer, 1,MAX, fp); //전체 읽기
    printf("%s", buffer);

    wc = count_word(buffer);
    printf("\n단어의 개수 : %d \n\n", wc);
	nc = count_numbner(buffer);
	printf("숫자의 개수 : %d \n\n", nc);
	uc = count_upper(buffer);
	printf("대문자의 개수 : %d \n\n", uc);

    fclose(fp);

	return 0;
}

int count_word(char* s)
{
	int i, wc = 0, waiting = 1;

	for (i = 0; s[i] != NULL; ++i)
		if (isalpha(s[i]))
		{
			if (waiting)
			{
				wc++;
				waiting = 0;
			}

		}
		else
			waiting = 1;
	return wc;
}
int count_numbner(char* s)
{
	int i, nc = 0, waiting = 1;

	for (i = 0; s[i] != NULL; ++i)
		if (isdigit(s[i]))
		{
			if (waiting)
			{
				nc++;
				waiting = 0;
			}

		}
		else
			waiting = 1;
	return nc;
}
int count_upper(char* s)
{
	int i, uc = 0, waiting = 1;

	for (i = 0; s[i] != NULL; ++i)
		if (isupper(s[i]))
		{
			if (waiting)
			{
				uc++;
				waiting = 0;
			}

		}
		else
			waiting = 1;
	return uc;
}