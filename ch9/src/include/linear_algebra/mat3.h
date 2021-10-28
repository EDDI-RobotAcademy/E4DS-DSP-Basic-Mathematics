#ifndef __MATRIX_3D_H__
#define __MATRIX_3D_H__

#include "linear_algebra/mat3.h"

#include <stdbool.h>

void init_mat(float (*A)[3]);
void print_mat(float (*R)[3]);
void add_mat(float (*A)[3], float (*B)[3], float (*R)[3]);
void sub_mat(float (*A)[3], float (*B)[3], float (*R)[3]);
void scale_mat(float scale_factor, float (*A)[3], float (*R)[3]);
void mul_mat(float (*A)[3], float (*B)[3], float (*R)[3]);
float det_mat(float (*A)[3]);
void trans_mat(float (*A)[3], float (*R)[3]);
void adj_mat(float (*A)[3], float (*R)[3]);
bool inv_mat(float (*A)[3], float (*R)[3]);
void molding_mat(float (*A)[3], float *ans, int idx, float (*R)[3]);
void crammer_formula(float (*A)[3], float *ans, float *xyz);
void print_vec3(float *vec);
void create_3x4_mat(float (*A)[3], float *ans, float (*R)[4]);
void print_3x4_mat(float (*R)[4]);
void adjust_3x4_mat(float (*A)[4], int idx, float (*R)[4]);
void finalize(float (*R)[4], float *xyz);
void gauss_elimination(float (*A)[3], float *ans, float *xyz);
void create_3x6_mat(float (*A)[3], float (*R)[6]);
void print_3x6_mat(float (*R)[6]);
void adjust_3x6_mat(float (*A)[6], int idx, float (*R)[6]);
void gauss_elim_mat(float (*A)[3], float (*R)[3]);

#endif
