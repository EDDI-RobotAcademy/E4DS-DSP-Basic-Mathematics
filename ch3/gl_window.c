#include <GL/glut.h>

void my_disp(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 300, 300);
	glColor3f(1.0, 0.0, 0.0);
	glFlush();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow("OGL Window Exam");
	glutDisplayFunc(my_disp);
	glutMainLoop();

	return 0;
}
