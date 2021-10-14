#ifndef __VECTOR_3D_H__
#define __VECTOR_3D_H__

#include <stdio.h>
#include <math.h>

typedef struct vector3d vec3;

struct vector3d
{
	float x;
	float y;
	float z;

	void (* add)(vec3, vec3, vec3 *);
	void (* sub)(vec3, vec3, vec3 *);
	void (* scale)(float, vec3, vec3 *);
	float (* dot)(vec3, vec3);
	void (* cross)(vec3, vec3, vec3 *);
	void (* print)(vec3);

	void (* gramschmidt)(vec3 *, vec3 *, vec3);
};

void vec3_add(vec3 a, vec3 b, vec3 *r);
void vec3_sub(vec3 a, vec3 b, vec3 *r);
void vec3_scale(float factor, vec3 a, vec3 *r);
float vec3_dot(vec3 a, vec3 b);
void vec3_cross(vec3 a, vec3 b, vec3 *r);
void print_vec3(vec3 r);
float magnitude(vec3 v);
void gramschmidt_normalization(vec3 *arr, vec3 *res, vec3 r);

#endif
