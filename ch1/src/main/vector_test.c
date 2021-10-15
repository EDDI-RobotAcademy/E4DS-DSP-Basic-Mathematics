#include "linear_algebra/vector_3d.h"
#include <stdio.h>

int main(void)
{
	int i;

	vec3 A = {3, 2, 1};
	vec3 B = {1, 1, 1};
	vec3 X = {1, 0, 0};
	vec3 Y = {0, 1, 0};
	vec3 unit_A = {};
	vec3 v[3] = {{0, 4, 0}, {2, 2, 1}, {1, 1, 1}};
	vec3 w[3] = {};
	vec3 R = {0, 0, 0, 
			  vec3_add, vec3_sub, vec3_scale,
			  vec3_dot, vec3_cross, print_vec3,
			  gramschmidt_normalization};

	printf("벡터의 크기\n");
	printf("Magnitude of Vector: %f\n", magnitude(A));

	printf("단위 벡터\n");
	// 단위 벡터 = 임의의 벡터 / 임의의 벡터 크기
	R.scale(1.0f / magnitude(A), A, &unit_A);
	print_vec3(unit_A);

	printf("단위 벡터의 크기\n");
	printf("Magnitude of Unit Vector: %f\n", magnitude(unit_A));

	printf("벡터 덧셈\n");
	R.add(A, B, &R);
	R.print(R);

	printf("벡터 뺄셈\n");
	R.sub(A, B, &R);
	R.print(R);

	printf("벡터 스케일링\n");
	R.scale(3, R, &R);
	R.print(R);

	printf("벡터 내적\n");
	// 수직 판정에 많이 활용
	printf("A dot B = %f\n", R.dot(A, B));

	printf("벡터 내적을 통한 수직 검사\n");
	printf("X dot Y = %f\n", R.dot(X, Y));

	printf("벡터 외적\n");
	R.cross(X, Y, &R);
	printf("A cross B = ");
	R.print(R);

	R.gramschmidt(v, w, R);
	// R.print(R);
	for (i = 0; i < 3; i++)
	{
		R.print(w[i]);
	}

	return 0;
}
