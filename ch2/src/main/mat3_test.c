#include "linear_algebra/mat3.h"

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(void)
{
	bool inv_flag;

	float test[3][3] = {{2.0, 0.0, 4.0}, {0.0, 3.0, 9.0}, {0.0, 0.0, 1.0}};
	float stimul[3][3] = {{2.0, 4.0, 4.0}, {6.0, 2.0, 2.0}, {4.0, 2.0, 4.0}};
	float ans[3] = {12.0, 16.0, 20.0};
	float xyz[3] = {};

	float A[3][3] = {};
	float B[3][3] = {};
	float R[3][3] = {};

	srand(time(NULL));

	printf("Init A Matrix\n");
	init_mat(A);
	print_mat(A);

	printf("Init B Matrix\n");
	init_mat(B);
	print_mat(B);

	printf("A + B Matrix\n");
	add_mat(A, B, R);
	print_mat(R);

	printf("A - B Matrix\n");
	sub_mat(A, B, R);
	print_mat(R);

	printf("Matrix Scale(A)\n");
	scale_mat(0.5, A, R);
	print_mat(R);

	printf("AB Matrix\n");
	mul_mat(A, B, R);
	print_mat(R);

	printf("det(A) = %f\n", det_mat(A));
	printf("det(B) = %f\n", det_mat(B));

	printf("\nA^T(Transpose) Matrix\n");
	trans_mat(A, R);
	print_mat(R);

	printf("B^T(Transpose) Matrix\n");
	trans_mat(B, R);
	print_mat(R);

	printf("A Inverse Matrix\n");
	inv_flag = inv_mat(A, R);
	if(inv_flag)
		print_mat(R);
	else
		printf("역행렬 없다!\n");

	printf("test Inverse Matrix\n");
	inv_flag = inv_mat(test, R);
	if(inv_flag)
		print_mat(R);
	else
		printf("역행렬 없다!\n");

	printf("크래머 공식 기반 연립 방정식 풀기!\n2x + 4y + 4z = 12\n6x + 2y + 2z = 16\n4x + 2y + 4z = 20\n");
	crammer_formula(stimul, ans, xyz);
	print_vec3(xyz);

	printf("가우스 소거법 기반 연립 방정식 풀기!(문제 위의 것과 동일함)\n");
	gauss_elimination(stimul, ans, xyz);
	print_vec3(xyz);

	printf("가우스 소거법으로 역행렬 구하기!\n");
	gauss_elim_mat(test, R);
	print_mat(R);

	return 0;
}
