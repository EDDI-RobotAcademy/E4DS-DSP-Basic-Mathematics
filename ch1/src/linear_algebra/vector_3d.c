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

float vec3_dot(vec3 a, vec3 b)
{
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

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
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void gramschmidt_normalization(vec3 *arr, vec3 *res, vec3 r)
{
	vec3 scale1 = {};
	vec3 scale2 = {};
	float dot1, mag1, dot2, mag2;

	mag1 = magnitude(arr[0]);
	r.scale(1.0 / mag1, arr[0], &res[0]);
	r.print(res[0]);

	mag1 = magnitude(res[0]);
	dot1 = r.dot(arr[1], res[0]);
	r.scale(dot1 * (1.0 / pow(mag1, 2.0)), res[0], &scale1);
	r.sub(arr[1], scale1, &res[1]);
	r.print(res[1]);

	dot1 = r.dot(arr[2], res[0]);
	r.scale(dot1 * (1.0 / pow(mag2, 2.0)), res[0], &scale1);
	r.sub(arr[2], scale2, &res[2]);

	mag2 = magnitude(res[1]);
	dot2 = r.dot(arr[2], res[1]);
	r.scale(dot2 * (1.0 / pow(mag2, 2.0)), res[1], &scale2);
	r.sub(res[2], scale2, &res[2]);
	r.print(res[2]);
}
