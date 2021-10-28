#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/glu.h>

float cx, cy, cz;
float tx, ty, tz = -10.0f;
float ux, uy = 1.0f, uz;

float sample = 0.1f;

float tmpz = -1.0f;

void InitLight(void)
{
	GLfloat mat_diffuse[] = {0.5, 0.4, 0.3, 1.0};
	GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat mat_ambient[] = {0.5, 0.4, 0.3, 1.0};
	GLfloat mat_shininess[] = {15.0};
	GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat light_diffuse[] = {0.8, 0.8, 0.8, 1.0};
	GLfloat light_ambient[] = {0.3, 0.3, 0.3, 1.0};
	GLfloat light_position[] = {-3, 6, 3.0, 0.0};
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_specular);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

void MyDisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, -1.0, 0.0, 1.0, 0.0);
	//gluLookAt(cx, cy, cz, tx, ty, tz, ux, uy, uz);
	//gluLookAt(cx, 10.0f, cz, tx, ty, tz, ux, uy, uz);

	// 카메라 위쪽에서 내려다보는 기준
	//gluLookAt(cx, 10.0f, 1.0f, tx, 5.0f, 0.0f, ux, uy, uz);
	//gluLookAt(cx, 8.0f, 1.0f, tx, 0.0f, 0.0f, ux, uy, uz);
	gluLookAt(cx, 10.0f, 1.0f, tx, 0.0f, 0.0f, ux, uy, uz);

#if 0
	tmpz += sample;
	glPushMatrix();
	// 위로 상승
	//glTranslatef(0.0f, tmpz, 0.0f);
	// 앞으로 전진
	//glTranslatef(tmpz, 0.0f, 0.0f);
	glutSolidTeapot(0.5);
	glPopMatrix();
#else
	//glutSolidTeapot(0.5);
#endif

	tmpz += sample;
	glPushMatrix();
	glTranslatef(0.0f, tmpz, 0.0f);
	glutSolidTeapot(0.5f);
	glPopMatrix();

	glFlush();
}

void MyReshape(int w, int h)
{
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-100.0, 100.0, -100.0, 100.0, -100.0, 100.0);
}

void TimerFunction(int value)
{
    glutPostRedisplay();
    glutTimerFunc(3,TimerFunction, 1);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("OpenGL Rendering Example");
	glClearColor(0.4, 0.4, 0.4, 0.0);
	glMatrixMode(GL_PROJECTION);
	InitLight();
	glutDisplayFunc(MyDisplay);
	glutReshapeFunc(MyReshape);

	glutTimerFunc(1000, TimerFunction, 1);
	glutMainLoop();
	return 0;
}
