
#define _CRT_SECURE_NO_WARNINGS //ǥ�� C�� secure ���� �����ϵ��� ����

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
	command: d: ���� ��ü�� ������
e: ������ �������� ���⸦ ���� ���� �����ϱ�
f: ���⸦ �������� ���� ������
g: ���� ������ Ư�� ���ڿ��� �ٸ� ���ڿ��� �ٲٱ�
h: �յڿ��� �о��� �� ������ ���� �������� ����ϱ�
q: ����
	*/

	FILE* fp = fopen("data.txt", "r");  //test������ r(�б�) ���� ����
	char buffer[MAX] = { 0, };

	fread(buffer, 1, MAX, fp); //��ü �б�
	printf("%s", buffer);
	printf("\n");


	while ((x != 'q') && (x != 'Q')) //�ݺ��� ����
	{

		printf("��ɾ� ����Ʈ\n");
		printf("g: Ư�� ���ڸ� ��� �ٸ� ���ڷ� �ٲٰ� ���\n");//��ɾ �ٽ� �����ϸ� �ҹ��ڷ�
		printf("f / F : ��� ���鿡 ������ ����(@, %, # �Ǵ� .)�� �ְ� ���\n");//��ɾ �ٽ� �����ϸ� �������ڸ� ��������
		printf("l / L : �ܾ��� ������ ���\n");
		printf("c / C : �빮�ڴ� �ҹ��ڷ�, �ҹ��ڴ� �빮�ڷ� �ٲٱ�\n");
		printf("a / A : ������ ���ڸ� �������� �� ���ھ� �̵��Ѵ�.\n");
		printf("q / Q : ���α׷��� ����\n");
		printf("��ɾ �Է��ϼ���: \n\n");
		x = _getch();

		if ((x == 'l') || (x == 'L'))//L����
		{
			int wc = count_word(s);
			printf("�ܾ��� ���� : % d \n\n", wc);
		}//L��

		if ((x == 'f') || (x == 'F'))//f����
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

			token = strtok(s, delimit); //���� ��ū���� �и� 

			while (token != NULL) //��ū �������� ���ġ
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
			printf("����: %s\n\n", s);
		}//f��

		if ((x == 'G') || (x == 'g'))//g����
		{

			token = strtok(s, delimit); //���� ��ū���� �и� 

			while (token != NULL) //��ū �������� ���ġ
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
			strcpy(s2, s3);//s3���� s2 �ʱ�ȭ
			printf("����: %s\n\n", s);
		}//g��

		if ((x == 'c') || (x == 'C'))//c����
		{

			while (s[I] != 0x0) {  // �ƽ�Ű �ڵ� �̿�
				if (s[I] >= 'A' && s[I] <= 'Z')
				{
					s[I] = s[I] + 32;// �빮�� -> �ҹ���
				}
				else if (s[I] >= 'a' && s[I] <= 'z')
				{
					s[I] = s[I] - 32;// �ҹ��� -> �빮��
				}
				I = I + 1;
			}
			I = 0;//I�ʱ�ȭ
			printf("����: %s\n\n", s);
		}//c��


		if ((x == 'a') || (x == 'A'))//a����
		{
			s2[0] = s[0];
			while (s[I] != 0x0) {

				s[I] = s[(I + 1)];



				I = I + 1;
			}
			s[(I - 2)] = s2[0];
			strcat(s, ".");
			I = 0;
			strcpy(s2, s3);//s3���� s2 �ʱ�ȭ
			printf("����: %s\n\n", s);
		}//a��


	}//�ݺ��� ��
	return 0;
}

