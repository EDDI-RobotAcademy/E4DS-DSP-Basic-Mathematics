#include "linear_algebra/vector_3d.h"

void vec3_add(vec3 a, vec3 b, vec3 *r)
{
	r->x = a.x + b.x;
	r->y = a.y + b.y;
	r->z = a.z + b.z;
}

void vec3_sub(vec3 a, vec3 b, vec3 *r)
{
	r->x = a.x - b.x;
	r->y = a.y - b.y;
	r->z = a.z - b.z;
}

void vec3_scale(float factor, vec3 a, vec3 *r)
{
	r->x = a.x * factor;
	r->y = a.y * factor;
	r->z = a.z * factor;
}

// x성분은 x 성분끼리
// y성분은 y 성분끼리
// z성분은 z 성분끼리
// 곱해서 각 결과들을 모두 합산하여 내적의 결과를 얻음
// A * B cos(theta) = 0 ---> cos(theta) = 0
float vec3_dot(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

#if 0
	i	j	k
	ax	ay	az
	bx	by	bz

	i(x축 벡터) * (ay * bz) - (az * by)
	j(y축 벡터) * (az * bx) - (ax * bz)
	k(z축 벡터) * (ax * by) - (ay * bx)
#endif

void vec3_cross(vec3 a, vec3 b, vec3 *r)
{
	r->x = a.y * b.z - a.z * b.y;
	r->y = a.z * b.x - a.x * b.z;
	r->z = a.x * b.y - a.y * b.x;
}

void print_vec3(vec3 r)
{
	printf("x = %f, y = %f, z = %f\n", r.x, r.y, r.z);
}

float magnitude(vec3 v)
{
	// -lm 컴파일시 옵션, math.h
	//     sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2))
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void gramschmidt_normalization(vec3 *arr, vec3 *res, vec3 r)
{
	vec3 scale1 = {};
	vec3 scale2 = {};
	float dot1, mag1, dot2, mag2;

	vec3 tmp = {};

	// arr[0] = {0, 4, 0} -> magnitude() = 4
	mag1 = magnitude(arr[0]);
	r.scale(1.0 / mag1, arr[0], &res[0]);
	printf("w0 = \n");
	r.print(res[0]);
	// res[0] = {0, 1, 0}

	mag1 = magnitude(res[0]);
	// v1 dot w0
	// {2, 2, 1} = v1, {0, 1, 0} = w0
	// dot1 = 2
	dot1 = r.dot(arr[1], res[0]);
	// [v1 dot w0 / |w0|^2] w0 = 2 * w0 = {0, 2, 0}
	r.scale(dot1 * (1.0 / pow(mag1, 2.0)), res[0], &scale1);
	// v1 - [v1 dot w0 / |w0|^2] w0 = w1
	// {2, 2, 1} - {0, 2, 0} = {2, 0, 1}
	r.sub(arr[1], scale1, &res[1]);
	printf("w1 = \n");
	r.print(res[1]);

	// w0, w1 cross
	r.cross(res[0], res[1], &tmp);
	printf("w0 cross w1 = \n");
	r.print(tmp);

	// v2 dot w0
	// {1, 1, 1} dot {0, 1, 0} = 1
	dot1 = r.dot(arr[2], res[0]);
	// mag1이 mag2로 잘못 기재되었음(값은 1이기 때문에 동작에 문제는 없음)
	//r.scale(dot1 * (1.0 / pow(mag2, 2.0)), res[0], &scale1);
	r.scale(dot1 * (1.0 / pow(mag1, 2.0)), res[0], &scale1);
	// v2 - w0
	// {1, 1, 1} - {0, 1, 0} = {1, 0, 1}
	//r.sub(arr[2], scale2, &res[2]);
	r.sub(arr[2], scale1, &res[2]);
	printf("w2 중간 = \n");
	r.print(res[2]);

	// sqrt(5) = mag2
	mag2 = magnitude(res[1]);
	// {1, 1, 1} dot {2, 0, 1} = 3
	dot2 = r.dot(arr[2], res[1]);
	// 3 / 5 {2, 0, 1}
	r.scale(dot2 * (1.0 / pow(mag2, 2.0)), res[1], &scale2);
	//r.sub(res[2], scale2, &res[2]);
	r.sub(res[2], scale2, &res[2]);
	// {1, 0, 1} - {6 / 5, 0, 3 / 5}
	// {-1 / 5, 0, 2 / 5}
	printf("w2 = \n");
	r.print(res[2]);
}
