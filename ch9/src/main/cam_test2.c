#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

GLUquadricObj *qobj;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 1.0, 1.0);
	gluSphere(qobj, 1.0, 20, 20);
	glutSwapBuffers();
}

void gfxinit(void)
{
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE);
}

void myReshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w / (GLfloat)h, 1.0, 10.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 5.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glTranslatef(0.0, 0.0, -1.0);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Simple");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(display);
	gfxinit();
	glutMainLoop();
	return 0;
}
