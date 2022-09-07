#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int A[2][2], B[2][2], temp[2][2];
int i, j;//반복문 변수

void rand_input(){
	
	srand(time(NULL));

	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			A[i][j] = rand() % 3;
			B[i][j] = rand() % 3;
		}
	}
}

void print_matrix()
{
	printf("matrix A:\tmatrix B:\n");
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d", A[i][j]);
		}
		printf("\t\t");
		for (j = 0; j < 3; j++) {
			printf("%d", B[i][j]);
		}
		printf("\n");
	}
}
void print_explain()
{
	printf("\m: 행렬의 곱셈\n\
a : 행렬의 덧셈\n\
d : 행렬의 뺄셈(행렬A-행렬B)\n\
r : 행렬식의 값(Determinant)\n\
t : 전치 행렬(Transposed matrix)과 그 행렬식의 값  입력한 2개의 행렬에 모두 적용한다.\n\
h : 3X3 행렬을 4X4 행렬로 변환하고 행렬식의 값(4행4열 행렬식 값) 출력\n\
s : 행렬의 값을 새로 랜덤하게 설정한다.\n\
q : 프로그램 종료\n");

}

void matrix_add()
{
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d ",A[i][j] + B[i][j]);
		}
		printf("\n");
	}
}
void matrix_sub()
{
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d ", A[i][j] - B[i][j]);
		}
		printf("\n");
	}

}
void matrix_mul()
{
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d ", A[i][0]*B[0][j]+ A[i][1] * B[1][j] + A[i][2] * B[2][j]);
		}
		printf("\n");
	}
}
void matrix_determinant(int a, int b)
{

}
void matrix_transpose(int a, int b)
{
	for (i = 0; i < 3; i++) {
		for (j = 0; j < 3; j++) {
			printf("%d ", A[j][i]);
		}
		printf("\t\t");
		for (j = 0; j < 3; j++) {
			printf("%d ", B[j][i]);
		}
		printf("\n");
	}
}
void matrix_33to44(int a, int b)
{
	int C[3][3];

}
int main()
{
	
	char command[1];

	rand_input(A, B);
	
	print_matrix(A, B);

	print_explain();
	scanf_s("%c", &command);

	while (command != 'q') {
		if (command == 'a') { matrix_add(A, B); }
		else if (command == 'd') { matrix_sub(A, B); }
		else if (command == 'm') { matrix_mul(A, B); }
		else if (command == 'r') { matrix_determinant(A, B); }
		else if (command == 't') { matrix_transpose(A, B); }
		else if (command == 'h') { matrix_33to44(A, B); }
		else if (command == 's') { rand_input(A, B); }
		print_explain();
		scanf_s("%c", &command);
	}

	return 0;
}
