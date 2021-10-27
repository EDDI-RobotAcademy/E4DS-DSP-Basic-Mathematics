#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

#define SLICE	360

void draw_uni_rand_hist(void);

float common_angles[5] = {15.0, 30.0, 45.0, 60.0, 75.0};
float freq_table[5] = {1000.0, 2400.0, 5000.0, 24000.0, 77000.0};

float theta = 0.0;

#define NMAX		10000000

int *sbuf;
int *hist_buf;

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

	draw_uni_rand_hist();

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

int get_sample(int num)
{
    int i;
    sbuf = (int *)calloc(num, sizeof(int));

    if(sbuf)
    {
        for(i = 0; i < num; i++)
        {
            sbuf[i] = rand();
        }   
    }
	else
		return 0;

    return 1;
}

int make_hist(int num)
{
    int i;
    // 85899346 * 50 = 4294967300
    // uint max      = 4294967295
    // 42949673 * 50 = 2147483650
    int width_range = 42949673;

    hist_buf = (int *)malloc(sizeof(int) * RAND_MAX / width_range);

	if(!hist_buf)
		return 0;

    memset(hist_buf, 0x0, (RAND_MAX / width_range) * sizeof(int));

    printf("num of hbuf elements = %d\n", RAND_MAX / width_range);

    for(i = 0; i < num; i++)
    {
        hist_buf[sbuf[i] / width_range]++;
    }

    return 1;
}

void print_hist(void)
{
	int i;
	int width_range = 42949673;
	int hist_range = RAND_MAX / width_range;

	for(i = 0; i < hist_range; i++)
		printf("hist_buf[%d] = %d\n", i, hist_buf[i]);
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

void draw_uni_rand_hist(void)
{
	float x2 = 0, y2, cx, cy;
	float *x_vertices, *y_vertices;
	float x_scale, y_scale;
	const float fix_x_coord = -100.0f;
	const float fix_y_coord = -90.0f;
	int num, i, cache = 0;

	srand(time(NULL));

	if(get_sample(NMAX))
	{
		if(make_hist(NMAX))
			printf("Success\n");
		else
			printf("Fail to make Hist Data\n");
	}
	else
		printf("Fail to make Sample\n");

	glBegin(GL_LINES);
		glVertex2f(-100, -90);
		glVertex2f(100, -90);
	glEnd();

	print_hist();

	num = (RAND_MAX / 42949673);
	x_scale = 200.0 / (float)num;
	x_vertices = (float *)malloc(sizeof(float *) * num);
	// find max 가 더효율적이나 그냥 상수로 가즈아
	y_scale = 180.0 / (float)210000.0;
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
	free(sbuf);
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
