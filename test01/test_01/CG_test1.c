#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int A[3][3], B[3][3], temp[3][3];
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
	printf("\nm: 행렬의 곱셈\n\
a : 행렬의 덧셈\n\
d : 행렬의 뺄셈(행렬A-행렬B)\n\
r : 행렬식의 값(Determinant)\n\
t : 전치 행렬(Transposed matrix)과 그 행렬식의 값  입력한 2개의 행렬에 모두 적용한다.\n\
h : 3X3 행렬을 4X4 행렬로 변환하고 행렬식의 값(4행4열 행렬식 값) 출력\n\
s : 행렬의 값을 새로 랜덤하게 설정한다.\n\
q : 프로그램 종료\n\
명령: ");

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
void matrix_determinant()
{
	int det_a, det_b;
	det_a = A[0][0] * A[1][1] * A[2][2] + A[0][1] * A[1][2] * A[2][0] + A[0][2] * A[1][0] * A[2][1] - (A[0][0] * A[1][2] * A[2][1] + A[0][1] * A[1][0] * A[2][2] + A[0][2] * A[1][1] * A[2][0]);
	det_b = B[0][0] * B[1][1] * B[2][2] + B[0][1] * B[1][2] * B[2][0] + B[0][2] * B[1][0] * B[2][1] - (B[0][0] * B[1][2] * B[2][1] + B[0][1] * B[1][0] * B[2][2] + B[0][2] * B[1][1] * B[2][0]);
	printf("matrix A: %d\tmatrix B: %d\n", det_a, det_b);
}
void matrix_transpose()
{
	printf("matrix A:\tmatrix B:\n");
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
	int Ato4[4][4], Bto4[4][4];
	
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			
			if (i == 3 && j == 3) {
				Ato4[i][j] = 1;
				Bto4[i][j] = 1;
			}
			else if (i == 3 || j == 3) {
				Ato4[i][j] = 0;
				Bto4[i][j] = 0;
			}
			else {
				Ato4[i][j] = A[i][j];
				Bto4[i][j] = B[i][j];
			}
		}
	}
	printf("4*4matrix A:\t4*4matrix B:\n");
	for (i = 0; i < 4; i++) {
		for (j = 0; j < 4; j++) {
			printf("%d", Ato4[i][j]);
		}
		printf("\t\t");
		for (j = 0; j < 4; j++) {
			printf("%d", Bto4[i][j]);
		}
		printf("\n");
	}
}
int main()
{
	
	char command;

	rand_input(A, B);
	
	print_matrix(A, B);

	command = 'b';
	while (command != 'q') {
		print_explain();
		scanf("%c", &command);
		getchar();
		printf("\n");
		if (command == 'a') { matrix_add(A, B); }
		else if (command == 'd') { matrix_sub(A, B); }
		else if (command == 'm') { matrix_mul(A, B); }
		else if (command == 'r') { matrix_determinant(A, B); }
		else if (command == 't') { matrix_transpose(A, B); }
		else if (command == 'h') { matrix_33to44(A, B); }
		else if (command == 's') { rand_input(A, B);
		print_matrix();
		}

		
	}

	return 0;
}