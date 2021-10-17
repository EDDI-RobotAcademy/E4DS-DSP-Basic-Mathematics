#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>

#include <unistd.h>
#include <signal.h>

#define SLICE	360

typedef struct _line_vertex line_vertex;
struct _line_vertex
{
	float x1;
	float y1;

	float x2;
	float y2;
};

float text_x = 64.0f, text_y = 60.0f;
const float TRANS_VAL = 100.0f;

line_vertex vertices[9] = {
	{ 50.0f, 50.0f, 50.0f, 30.0f },
	{ 50.0f, 50.0f, 55.0f, 50.0f },
	{ 75.0f, 50.0f, 80.0f, 50.0f },
	{ 80.0f, 50.0f, 80.0f, 30.0f },
	{ 55.0f, 55.0f, 55.0f, 45.0f },
	{ 55.0f, 55.0f, 75.0f, 55.0f },
	{ 75.0f, 55.0f, 75.0f, 45.0f },
	{ 55.0f, 45.0f, 75.0f, 45.0f }
};

void timer_handler(int signo)
{
	int i;

	for (i = 0; i < 8; i++)
	{
		vertices[i].x1 -= TRANS_VAL;
		vertices[i].x2 -= TRANS_VAL;
	}

	text_x -= TRANS_VAL;

	glutPostRedisplay();
}

void drawString (char *s)
{
	unsigned int i;

	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]);
}

void drawStringBig (char *s)
{
	unsigned int i;

	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, s[i]);
}

void draw_resistance(void)
{
	int i;
	static char label[100];

	glColor3f(1, 0, 1);

	glBegin(GL_LINES);
		for (i = 0; i < 8; i++)
		{
			glVertex2f(vertices[i].x1, vertices[i].y1);
			glVertex2f(vertices[i].x2, vertices[i].y2);
		}
	glEnd();

	sprintf (label, "R");
	glRasterPos2f (text_x, text_y);
	drawStringBig (label);
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

	draw_resistance();

	glFlush();
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

	signal(SIGALRM, timer_handler);
	alarm(5);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}
