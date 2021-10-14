#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

#define SLICE	360

void draw_A_add_B(void)
{
	glColor3f(1, 0, 1);

	glBegin(GL_LINE_LOOP);
		glVertex3f(30.0, 15.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(15.0, 30.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glColor3f(1, 1, 0);

	glBegin(GL_LINE_LOOP);
		glVertex3f(45.0, 45.0, 0.0);
		glVertex3f(30.0, 15.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(45.0, 45.0, 0.0);
		glVertex3f(15.0, 30.0, 0.0);
	glEnd();

	glColor3f(0, 0, 1);

	glBegin(GL_LINE_LOOP);
		glVertex3f(45.0, 45.0, 0.0);
		glVertex3f(0.0, 0.0, 0.0);
	glEnd();
}

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

	draw_A_add_B();

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
