#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

int glob = 2;
int count = 1;

#define SLICE			(1024)
#define HALF_SLICE		(SLICE >> 1)
#define QUAD_SLICE		(SLICE >> 2)
#define CALC_ORDER		((HALF_SLICE) + 1)
#define CALC_HEIGHT		(SLICE - 3)

#define SAMPLE_FREQ		(12000000000.0)
#define SAMPLE_PERIOD		(1.0 / SAMPLE_FREQ)

#define CALC_5G_2PI		(10000000000 * M_PI)
//#define CALC_2_4G_2PI		(4800000000 * M_PI)
#define CALC_NOISE_2PI		(1536000000 * M_PI)

typedef struct complex
{
        double re; 
        double im; 
} c;

void draw_omega_sin(void);
void draw_spectrum(void);

float common_angles[5] = {15.0, 30.0, 45.0, 60.0, 75.0};
float freq_table[5] = {1000.0, 2400.0, 5000.0, 24000.0, 77000.0};

float theta = 0.0;

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	glColor3f(1, 0, 0);

	glBegin(GL_LINE_LOOP);
	glVertex3f(100.0, 0.0, 0.0);
        glVertex3f(-100.0, 0.0, 0.0);
        glEnd();

        glColor3f(0.0, 1.0, 0.0);

        glBegin(GL_LINE_LOOP);
        glVertex3f(0.0, 100.0, 0.0);
        glVertex3f(0.0, -100.0, 0.0);
        glEnd();

	//draw_omega_sin();
	draw_spectrum();
	glutSwapBuffers();
}

void reshape(int w, int h)
{
        GLfloat n_range = 100.0f;

        if(h == 0)
                h = 1;

        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();

        if(w <= h)
                glOrtho(-n_range, n_range, -n_range * h / w, n_range * h / w, -n_range, n_range);
        else
                glOrtho(-n_range * w / h, n_range * w / h, -n_range, n_range, -n_range, n_range);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
			exit(0);
			break;
	}
}

void set_rand_amplitude(float *amp)
{
	*amp = rand() % 3 + 3;
}

void set_angle_with_common_angles(float *angle)
{
	*angle = common_angles[rand() % 5];
}

void angle2radian(float *angle, float *radian)
{
	*radian = *angle * M_PI / 180.0;
}

void radian2angle(float *angle, float *radian)
{
	*angle = *radian * 180.0 / M_PI;
}

void set_rand_frequency(float *freq)
{
	*freq = freq_table[rand() % 5];
}

void calc_period(float *freq, float *period)
{
	*period = 1 / (*freq);
}

void calc_angular_velocity(float *freq, float *ang_vel)
{
	*ang_vel = 2 * M_PI * (*freq);
}

float get_step(float slice, float period)
{
	return period / slice;
}

static double rt_hypotd_snf(double u0, double u1)
{
	double y;
	double a;
	double b;
	a = fabs(u0);
	b = fabs(u1);
	if (a < b) {
		a /= b;
		y = b * sqrt(a * a + 1.0);
	} else if (a > b) {
		b /= a;
		y = a * sqrt(b * b + 1.0);
	//} else if (rtIsNaN(b)) {
	} else if(b == 0.0){
		y = b;
	} else {
		y = a * 1.4142135623730951;
	}

	return y;
}

void find_frequency(c X[SLICE], double R[CALC_ORDER], double f[HALF_SLICE])
{
	double P2[SLICE];
	c y[SLICE];
	int k;
	for (k = 0; k < SLICE; k++) {
#if 0
		if (X[k].im == 0.0) {
			y[k].re = X[k].re / 256.0;
			y[k].im = 0.0;
		} else if (X[k].re == 0.0) {
			y[k].re = 0.0;
			y[k].im = X[k].im / 256.0;
		} else {
#endif
		y[k].re = X[k].re / SLICE;
		y[k].im = X[k].im / SLICE;
//		}

		P2[k] = rt_hypotd_snf(y[k].re, y[k].im);
		//printf("P2[%d] = %lf\n", k, P2[k]);
	}

	memcpy(&R[0], &P2[0], CALC_ORDER * sizeof(double));
	for (k = 0; k < HALF_SLICE - 1; k++) {
		R[1 + k] = 2.0 * P2[1 + k];
		//printf("R[%d] = %lf\n", k + 1, R[k + 1]);
	}

	for(k = 0; k < CALC_ORDER; k++)
	{
		f[k] = SAMPLE_FREQ * k / SLICE;
		//printf("f[%d] frequency = %lf\n", k, f[k]);
	}
}

void draw_spectrum(void)
{
	float t, step = 0.0;
	float period, freq = SLICE;
	double rf[CALC_ORDER];
	double f[CALC_ORDER];

	c y[SLICE] = {0};
        int ix;
        int ju;
        int iy;
        int i;
        double x[SLICE] = {0};

	int tst;
        double temp_re;
        double temp_im;
        int iheight;
        int istart;
        int j;
        double twid_re;
        double dv0[CALC_ORDER] = {0};

	double twid_im;
	double dv1[CALC_ORDER] = {0};

	int ihi;

	calc_period(&freq, &period);
	step = get_step(SLICE, period);
	// period / slice

	if(t > period)
		t = 0.0;

	t = 0.0;
	for(i = 0; i < SLICE; t += SAMPLE_PERIOD, i++)
	{
		x[i] = 10 * sin(CALC_5G_2PI * t) + 5 * cos(CALC_NOISE_2PI * t);
		//x[i] = 20 * sin(CALC_200_PI_STEP * t) + 17 * cos(CALC_150_PI_STEP * t) - 18 * sin(CALC_120_PI_STEP * t);
		//x[i] = 20 * sin(200 * M_PI * t) + 17 * cos(150 * M_PI * t) - 18 * sin(120 * M_PI * t);
		//printf("x[%d] = %f\n", i++, x[i]);
		//printf("exp.cosx[%d] = %f\n", i, exp.cosx[i]);
		//printf("exp.isinx[%d] = %f\n", i, exp.isinx[i]);
		//printf("res_real[%d] = %f\n", i, res_real[i]);
		//printf("res_real = %f\n", res_real);
		//printf("res_image = %f\n", res_image);
	}

        t = 0;
	step = 2 * M_PI / SLICE;

        for(i = 0; i < CALC_ORDER; i++)
        {
                dv0[i] = cos(t);
                dv1[i] = -sin(t);
                t += step;
        }

	t = 0;

	ix = 0;
	ju = 0;
	iy = 0;
	//printf("Before Reverse Order\n");
	for (i = 0; i < SLICE - 1; i++) {
		y[iy].re = x[ix];
		y[iy].im = 0.0;

		//printf("y[%d].re = %lf\t", iy, y[iy].re);
		//printf("y[%d].im = %lf\n", iy, y[iy].im);

		iy = SLICE;
		tst = 1;
		while (tst) {
			iy >>= 1;
			ju ^= iy;
			tst = ((ju & iy) == 0);
		}

		iy = ju;
		ix++;
	}
	y[iy].re = x[ix];
	y[iy].im = 0.0;

#if 0
	printf("\nAfter Reverse Order\n");
	for(i = 0; i < SLICE; i++)
	{
		printf("y[%d].re = %lf\t", i, y[i].re);
		printf("y[%d].im = %lf\n", i, y[i].im);
	}
	printf("\nN-2 First Butterfly\n");
#endif

	for (i = 0; i <= SLICE - 1; i += 2) {
		temp_re = y[i + 1].re;
		temp_im = y[i + 1].im;
		y[i + 1].re = y[i].re - y[i + 1].re;
		y[i + 1].im = y[i].im - y[i + 1].im;
		y[i].re += temp_re;
		y[i].im += temp_im;

#if 0
		printf("y[%d].re = %lf\t", i, y[i].re);
		printf("y[%d].im = %lf\n", i, y[i].im);
		printf("y[%d].re = %lf\t", i+1, y[i+1].re);
		printf("y[%d].im = %lf\n", i+1, y[i+1].im);
#endif
	}

	iy = 2;
	ix = 4;
	ju = QUAD_SLICE;
	iheight = CALC_HEIGHT;
	glob *= 2;
	while (ju > 0) {
		// 0 ~ 4
		//printf("\nN-%d Butterfly(처음은 짝수 오더)\n", glob);
		for (i = 0; i < iheight; i += ix) {
			temp_re = y[i + iy].re;
			temp_im = y[i + iy].im;
			y[i + iy].re = y[i].re - temp_re;
			y[i + iy].im = y[i].im - temp_im;
			y[i].re += temp_re;
			y[i].im += temp_im;

#if 0
			printf("y[%d].re = %lf\t", i, y[i].re);
			printf("y[%d].im = %lf\n", i, y[i].im);
			printf("y[%d].re = %lf\t", i+iy, y[i+iy].re);
			printf("y[%d].im = %lf\n", i+iy, y[i+iy].im);
#endif
		}
//		printf("\n");

		istart = 1;
//		printf("\nN-%d Butterfly(처음은 홀수 오더)\n", glob);
		for (j = ju; j < HALF_SLICE; j += ju) {
#if 0
			printf("twid_re = dv0 =  cos(2 * pi * f * t / fftN의절반)\n");
			printf("twid_im = dv1 = -sin(2 * pi * f * t / fftN의절반)\n");
#endif
			twid_re = dv0[j];
			twid_im = dv1[j];
			i = istart;
			ihi = istart + iheight;
			while (i < ihi) {
				temp_re = twid_re * y[i + iy].re - twid_im * y[i + iy].im;
				temp_im = twid_re * y[i + iy].im + twid_im * y[i + iy].re;
				y[i + iy].re = y[i].re - temp_re;
				y[i + iy].im = y[i].im - temp_im;
				y[i].re += temp_re;
				y[i].im += temp_im;

#if 0
				printf("y[%d].re = %lf\t", i, y[i].re / HALF_SLICE);
				printf("y[%d].im = %lf\n", i, y[i].im / HALF_SLICE);
				printf("y[%d].re = %lf\t", i+iy, y[i+iy].re / HALF_SLICE);
				printf("y[%d].im = %lf\n", i+iy, y[i+iy].im / HALF_SLICE);
#endif

				i += ix;
			}

			istart++;
		}

		ju /= 2;
		iy = ix;
		ix <<= 1;
		iheight -= iy;

		if(ju > 0)
		{
			printf("\nN-%d Butterfly 완료\nN-%d Butterfly 시작", glob, glob * 2);
			glob *= 2;
		}
		else
			printf("\nN-%d Butterfly 완료\n", glob * count);
	}

#if 0
	for(i = 0; i < SLICE; i++)
	{
		printf("x[%d] = %lf\n", i, x[i]);
		printf("y[%d].re = %lf\t", i, y[i].re / HALF_SLICE);
		printf("y[%d].im = %lf\n", i, y[i].im / HALF_SLICE);
	}
#endif

	printf("CALC_5G_2PI = %lf\n", CALC_5G_2PI);
	printf("CALC_NOISE_2PI = %lf\n", CALC_NOISE_2PI);
	printf("TIME_STEP = %lf\n", SAMPLE_PERIOD);

#if 1
	find_frequency(y, rf, f);

	for(i = 0; i < CALC_ORDER; i++)
		printf("rf[%d] = %lf, f[%d] = %lf\n", i, rf[i], i, f[i]);

//	for(i = 0; i < CALC_ORDER; i++)
//		printf("f[%d] = %lf\n", i, rf[i]);
#endif

	//printf("OK");

	for(i = 0; i < CALC_ORDER; i++)
	{
#if 0
		if(fabs(y[i].re) > 1 || fabs(y[i].im) > 1)
		{
			glBegin(GL_LINE_STRIP);
			glVertex2f(-i, y[i].re / HALF_SLICE);
			glVertex2f(-i, 0);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glVertex2f(-i, y[i].im / HALF_SLICE);
			glVertex2f(-i, 0);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glVertex2f(i, y[i].re / HALF_SLICE);
			glVertex2f(i, 0);
			glEnd();

			glBegin(GL_LINE_STRIP);
			glVertex2f(i, y[i].im / HALF_SLICE);
			glVertex2f(i, 0);
			glEnd();
		}
		else
			continue;
#endif
		glBegin(GL_LINE_STRIP);
		glVertex2f(-i*0.2, rf[i] * 5);
		glVertex2f(-i*0.2, 0);
		glEnd();

		glBegin(GL_LINE_STRIP);
		glVertex2f(i*0.2, rf[i] * 5);
		glVertex2f(i*0.2, 0);
		glEnd();
	}
	glob = 2;
}

int main(int argc, char **argv)
{
	float amplitude, angle, period, frequency, radian, angular_velocity;
	float step = 0.0;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(400, 200);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Digital Signal Processing");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}
