#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

#define SLICE	360

void draw_y_x(void);

void display(void)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

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

	draw_y_x();
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

void draw_y_x(void)
{
	float t = -100.0, step = 0.01;
	float x = 0, x2 = 0, y2, cx, cy;
	float tmp;
	int cache = 0;

	if(t > 100)
		t = 0.0;

	glBegin(GL_LINES);
	for(; ; t += step)
	{
		if(t > 100)
			break;

		y2 = t;

		if(cache)
		{
			// y'', y', y
			// z^-2, z^-1, z
			glVertex2f(cx, cy);	// 이전값
			glVertex2f(t, y2);	// 현재값

			// 이전 좌표 ~ 현재 좌표로 선을 긋는다.
			// GSL - GNU Scientific Library() - 미분방정식 라이브러리
			// 1계, 2계, RL Circuit Simulator
		}

		cache = 1;
		cx = t;
		cy = y2;
		printf("t = %f, y2 = %f\n", t, y2);
	}
	glEnd();
}

int main(int argc, char **argv)
{
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
