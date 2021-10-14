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

    // set matrix mode
    glMatrixMode(GL_MODELVIEW);
    // clear model view matrix
    glLoadIdentity();
    // multiply view matrix to current matrix
    gluLookAt(3.0, 3.0, 3.0-4.5, 0.0, 0.0,-4.5,0,1,0);

    // ******
    glPushMatrix();

    // glLoadIdentity(); <---- delete

    glTranslatef(0.0, 0.0, -4.5);

    glBegin(GL_LINES);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(100.0, 0.0, 0.0);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 100.0, 0.0);

    glColor3f (1.0, 1.0, 0.0);
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 100.0);
    glEnd();

    glPopMatrix();

    // traslate the draw by z = -4.0
    // Note this when you decrease z like -8.0 the drawing will looks far , or smaller.
    glTranslatef(0.0,0.0,-4.5);
    // Red color used to draw.
    glColor3f(0.8, 0.2, 0.1);

    // rotation about X axis
    glRotatef(30.0,1.0,0.0,0.0);
    // rotation about Y axis
    glRotatef(30.0,0.0,1.0,0.0);
    // rotation about Z axis
    glRotatef(30.0,0.0,0.0,1.0);

    // scaling transfomation
    glScalef(1.0,1.0,1.0);
    // built-in (glut library) function , draw you a Cone.

    // move the peak of the cone to the origin
    glTranslatef(0.0, 0.0, -100.0);

	// 반경, 높이, 원을 그리는 조각, 원점부터 끝점까지 그리는 조각
	glutWireCone(10, 100, 100, 100);
    //glutSolidCone(10,100,1000,100);

    //glFlush();
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
