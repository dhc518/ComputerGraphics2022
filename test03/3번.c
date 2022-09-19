#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#pragma warning	(disable:6386)

void Add_Space(char** s, int n)
{
	int size = (int)strlen(*s);
	int new_size = size + n + 1;
	int i = 0, j = 0;
	char* temp = {0};
	temp = (char*)malloc(sizeof(char*) * (new_size));

	if (temp != NULL)
	{
		memset(temp, '@', sizeof(char) * (new_size));
		temp[new_size - 1] = 0;
	//	printf("%s\n", temp);
		while (temp[i] != 0)
		{
			if ((i % 5) == 0 || (i % 5) == 1 || (i % 5) == 2)
			{
				temp[i] = (*s)[j];
				j++;
			}
			else
				temp[i] = '@';
			i++;
		}

		free(*s);
		*s = temp;
		temp = 0;
	}
}
void Delete_Space(char** s)
{
	int size = (int)strlen(*s);
	int new_size = 0;
	int Spa_Num = 0;
	char* temp;

	int i = 0, j = 0;

	while ((* s)[i] != 0)
	{
		if ((* s)[i] == '@')
			Spa_Num++;
		i++;
	}
	i = 0;

	new_size = size - Spa_Num + 1;
	temp = (char*)malloc(sizeof(char) * (new_size));

	if (temp != NULL)
	{
		memset(temp, '@', sizeof(char) * (new_size));
		temp[new_size - 1] = 0;

		while (temp[i] != 0)
		{
			if ((*s)[j] != '@')
			{
				temp[i] = (*s)[j];
				i++;
			}

			j++;
		}

		free(*s);
		*s = temp;
	}

}
void Reverse(char** s, int a, int b)
{
	char c;
	if (b - a == 2)
	{
		if ((*s)[a] == ' ' && (*s)[b] != ' ')
		{
			c = (*s)[b];
			(*s)[b] = (*s)[(b + a) / 2];
			(*s)[(b + a) / 2] = c;
		}
		else if ((*s)[b] == ' ' && (*s)[a] != ' ')
		{
			c = (*s)[a];
			(*s)[a] = (*s)[(b+a) / 2];
			(*s)[(b + a) / 2] = c;
		}
		else
		{
			c = (*s)[a];
			(*s)[a] = (*s)[b];
			(*s)[b] = c;
		}
	}
	else if( b - a == 1)
	{
		c = (*s)[a];
		(*s)[a] = (*s)[b];
		(*s)[b] = c;
	}
}
void Reverse_Word(char** s)
{
	bool IsSpace = true;
	int i = 0;
	int a = 0, b = 0;
	
	while ((*s)[i] != 0)
	{
		if ((*s)[i] == '@')
		{
			if (IsSpace == false)
			{
				b = i - 1;
				IsSpace = true;

				Reverse(s, a, b);
			}
		}
		else
		{
			if (IsSpace == true)
			{
				IsSpace = false;
				a = i;
			}
		}
		i++;
	} 
	b = i - 1;
	Reverse(s, a, b);
}
void ChangeChar(char** s, char a, char b)
{
	int i = 0;
	while ((*s)[i] != 0)
	{
		if ((*s)[i] == a)
		{
			(*s)[i] = b;
		}
		i++;
	}
}

void ReverseAtoB(char** s, int a, int b)
{
	int n = b - a + 1;
	char *c = (char *)malloc(sizeof(char) * (n + 1));
	
	if (c != NULL)
	{
		memset(c, 0, sizeof(char) * (n + 1));
		for (int i = 0; i < n; i++)
		{
			c[i] = (*s)[b - i];
		}
		for (int i = 0; i < n; i++)
		{
			(*s)[a + i] = c[i];
		}
		free(c);
	}
	
}

int main()
{
	FILE* fp = NULL;
	char filename[40];
	char file_buff[100];
	char* s[10];
	char command;
	int sl = 0, s_wl = 0;		//s_wl: 특수문자가 들어가는 개수 (e문제)
	char s_buf[100];
	bool IsE = false;

	int n = 0;

	char ReverseS[10][100] = {0};

	char ReverseW[10][20] = { 0 };
	bool IsSameReverse = true;
	int RW_number = 0;
	char ch_c1, ch_c2;

	int a = 0; int b = 0;

	memset(filename, 0, sizeof(filename));

	//printf("input data file name: ");
	//gets_s(filename, 40);
	if (0 == fopen_s(&fp, "data.txt", "r"))
	{
		for (int i = 0; i < 10; i++)
		{
			fgets(file_buff, sizeof(file_buff), fp);
			s[i] = (char*)malloc(sizeof(file_buff) + 1);
			if (s[i] == NULL)	return 0;
			else if (s[i] != NULL)
				strcpy_s(s[i], sizeof(file_buff), file_buff);
			memset(file_buff, 0, sizeof(file_buff));
			s[i][strlen(s[i]) - 1] = 0;
			printf("%s\n", s[i]);
		}
		while (1)
		{
			printf("\nd: 문장 전체 뒤집기\n");
			printf("e: 동일한 간격으로 띄어쓰기를 일정 개수 삽입하기\n");
			printf("f: 띄어쓰기를 기준으로 문자 뒤집기\n");
			printf("g: 문자 내부의 특정 문자열을 다른 문자열로 바꾸기\n");
			printf("h: 앞뒤에서 읽었을 때 어디까지 같은 문자인기 출력\n");
			printf("q: 종료\n");
			printf("명령: ");
			scanf_s("%c", &command, (int)sizeof(command));
			char c = getchar();
			switch (command)
			{
			case 'd':
				for (int i = 0; i < 10; i++)
				{
					sl = (int)strlen(s[i]);

					for (int j = sl - 1; j >= 0; j--)
					{
						s_buf[sl - 1 - j] = s[i][j];
					}
					for (int j = 0; j < sl; j++)
					{
						if(s_buf != NULL)s[i][j] = s_buf[j];
					}
				}
				for (int i = 0; i < 10; i++)
				{
					printf("%s\n", s[i]);
				}
				break;
			case 'e':
				//특수문자가 들어갈 공간 확보
				if (IsE == false)
				{
					for (int i = 0; i < 10; i++)
					{
						sl = (int)strlen(s[i]);		// sl : 문자열에서 문자의 개수(띄어쓰기 포함)
						if (sl % 3 != 0)
							s_wl = sl / 3 * 2;
						else
							s_wl = (sl / 3 - 1) * 2;
						Add_Space(&s[i], s_wl);
					}
					for (int i = 0; i < 10; i++)
					{
						printf("%s\n", s[i]);
					}

					IsE = true;
				}
				else
				{
					for (int i = 0; i < 10; i++)
					{
						Delete_Space(&s[i]);
					}
					for (int i = 0; i < 10; i++)
					{
						printf("%s\n", s[i]);
					}
					IsE = false;
				}
				break;
			case 'f':
				if (IsE == true)
				{
					for (int i = 0; i < 10; i++)
					{
						Reverse_Word(&s[i]);
					}
				}
				else
				{
					for (int i = 0; i < 10; i++)
					{
						n = 0; a = 0; b = 0;

						while (s[i][n] != 0)
						{
							if (s[i][n] == ' ')
							{
								b = n - 1;
								ReverseAtoB(&s[i], a, b);
								a = n + 1;
							}
							n++;
						}
						b = n - 1;
						ReverseAtoB(&s[i], a, b);

					}
				}
				for (int i = 0; i < 10; i++)
				{
					printf("%s\n", s[i]);
				}
				break;
			case 'g':
				scanf_s("%c %c", &ch_c1, (int)sizeof(char), &ch_c2, (int)sizeof(char));
				while (getchar() != '\n');
				printf("%c, %c \n", ch_c1, ch_c2);
				for (int i = 0; i < 10; i++)
				{
					ChangeChar(&s[i], ch_c1, ch_c2);
				}
				for (int i = 0; i < 10; i++)
				{
					printf("%s\n", s[i]);
				}
				break;
			case 'h':
				for (int i = 0; i < 10; i++)
				{
					sl = (int)strlen(s[i]);
					 
					for (int j = sl - 1; j >= 0; j--)
					{
						ReverseS[i][sl - 1 - j] = s[i][j];
					}
				}
				for (int i = 0; i < 10; i++)
				{
					printf("%s\n", ReverseS[i]);
				}
				for (int i = 0; i < 10; i++)
				{
					IsSameReverse = true;
					RW_number = 0;
					for (int j = 0; j < (strlen(s[i]) / 2); j++)
					{		
						if (s[i][j] == ReverseS[i][j])
						{
							if (IsSameReverse == true)
							{
								ReverseW[i][RW_number] = s[i][j];
								RW_number++;
							}
						}
						else
						{
							IsSameReverse = false;
						}
					}

				}

				memset(ReverseS, 0, sizeof(ReverseS));

				for (int i = 0; i < 10; i++)
				{
					printf("%s: %s\n", s[i], ReverseW[i]);
				}

				break;
			case 'q':
				fclose(fp);
				for (int i = 0; i < 10; i++)
					free(s[i]);
				return 0;
				break;
			}
		}
		

		fclose(fp);
		for (int i = 0; i < 10; i++)
		{
			if (s[i] != NULL)
				free(s[i]);
		}
	}

	
	return 0;
}