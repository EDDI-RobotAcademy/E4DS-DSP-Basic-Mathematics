#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

#define SLICE	360

void draw_gauss_rand_hist(void);

float common_angles[5] = {15.0, 30.0, 45.0, 60.0, 75.0};
float freq_table[5] = {1000.0, 2400.0, 5000.0, 24000.0, 77000.0};

float theta = 0.0;

#define NMAX		10000000

float *hist_buf;

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glColor3f(1, 0, 0);
	glLineWidth(5);

	glBegin(GL_LINE_LOOP);
		glVertex3f(120.0, -90.0, 0.0);
        glVertex3f(-120.0, -90.0, 0.0);
    glEnd();

    glBegin(GL_LINE_LOOP);
        glVertex3f(0.0, 100.0, 0.0);
        glVertex3f(0.0, -100.0, 0.0);
    glEnd();

	glColor3f(1, 1, 0.5);
	glLineWidth(1);

	draw_gauss_rand_hist();

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

int get_sample(int num, double mean, double std_dev)
{
    int i, idx, cnt = 0, range = 20;
	double width_range = 0.05;

	printf("num of hbuf elements = %d\n", range);

	hist_buf = (float *)malloc(sizeof(float) * range);
	if(!hist_buf)
		return 0;

	memset(hist_buf, 0x0, sizeof(float) * range);

	srand(time(NULL));

	for(i = 0; i < num; i++)
	{
		double x, y, r;

		do
		{
			x = 2.0 * rand() / RAND_MAX - 1;
			y = 2.0 * rand() / RAND_MAX - 1;

			r = x * x + y * y;
		}
		while (r == 0.0 || r > 1.0);

		double d = sqrt(-2.0 * log(r) / r);
		double n1 = x * d;
		double result = n1 * std_dev + mean;
		idx = result / width_range;

		if(idx >= range || idx < 0)
		{
			cnt++;
			continue;
		}

		hist_buf[idx]++;
	}

	printf("Outlier: idx overflow = %d\n", cnt);

    return 1;
}

void print_hist(void)
{
	int i;
	float width_range = 0.05;;
	int hist_range = 20;

	for(i = 0; i < hist_range; i++)
		printf("hist_buf[%d] = %f\n", i, hist_buf[i]);
}

void draw_finalize(float *x, float *y, int num)
{
	int i;

	glBegin(GL_LINES);
		glVertex2f(-100, y[0]);
		glVertex2f(x[0], y[0]);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(x[0], y[0]);
		glVertex2f(x[0], -90);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(-100, y[0]);
		glVertex2f(-100, -90);
	glEnd();

	for(i = 1; i < num - 1; i++)
	{
		glBegin(GL_LINES);
			glVertex2f(x[i], y[i]);
			glVertex2f(x[i - 1], y[i]);
		glEnd();

		glBegin(GL_LINES);
			glVertex2f(x[i], y[i]);
			glVertex2f(x[i], -90);
		glEnd();

		glBegin(GL_LINES);
			glVertex2f(x[i - 1], y[i]);
			glVertex2f(x[i - 1], -90);
		glEnd();
	}

	glBegin(GL_LINES);
		glVertex2f(x[i], y[i]);
		glVertex2f(x[i], -90);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(x[i], y[i]);
		glVertex2f(x[i - 1], y[i]);
	glEnd();

	glBegin(GL_LINES);
		glVertex2f(x[i - 1], y[i]);
		glVertex2f(x[i - 1], -90);
	glEnd();
}

void draw_gauss_rand_hist(void)
{
	float x2 = 0, y2, cx, cy;
	float *x_vertices, *y_vertices;
	float x_scale, y_scale;
	const float fix_x_coord = -100.0f;
	const float fix_y_coord = -90.0f;
	int num, i, cache = 0;

	if(get_sample(NMAX, 0.5, 0.2))
		printf("Success\n");
	else
	{
		printf("Fail to make Hist Data\n");
		exit(-1);
	}

	glBegin(GL_LINES);
		glVertex2f(-100, -90);
		glVertex2f(100, -90);
	glEnd();

	print_hist();

	num = 20;
	x_scale = 200.0 / (float)num;
	x_vertices = (float *)malloc(sizeof(float *) * num);
	// find max 가 더효율적이나 그냥 상수로 가즈아
	y_scale = 180.0 / (float)1000000.0;
	y_vertices = (float *)malloc(sizeof(float *) * num);

	for(i = 0; i < num; i++)
	{
		x_vertices[i] = fix_x_coord + x_scale * (i + 1);
		y_vertices[i] = fix_y_coord + y_scale * hist_buf[i];
		printf("chk x_vert[%d] = %f, y_vert[%d] = %f\n", i, x_vertices[i], i, y_vertices[i]);
	}

	draw_finalize(x_vertices, y_vertices, num);
}

void safety_exit(int signo)
{
	printf("Exit Safely\n");
	free(hist_buf);
	exit(0);
}

int main(int argc, char **argv)
{
	signal(SIGINT, safety_exit);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(1200, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Digital Signal Processing");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}
