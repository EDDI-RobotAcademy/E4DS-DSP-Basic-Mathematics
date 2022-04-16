#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

#define SLICE	360

void draw_resistance(void)
{
	glColor3f(1, 0, 1);

#if 0
	파일을 읽어서 해도 되지만
	로직 자체에 집중할 시간이 부족해지므로
	그냥 구조체를 만들고 구조체에 값을 박아놓고 해보면 됩니다.

	이와 같은 vertex 배열의 개수
	x, y, z를 표현할 수 있는 vertex 포인터 (갯수가 가변이니까)
	어디랑 어디가 연결되는지에 대한 정보가 필요함 (인덱스)
	- closed 되는 구성의 성분들끼리 모아놓고
	  opened 되는 구성의 성분들은 별도로 관리함

	- (시작점, 끝점) 방식의 관리
	  for 루프 돌릴때 특별히 생각할 사항이 없어진다는 이점이 있음
	  반쯤은 무지성으로 만들어도 됨
	  메모리 낭비가 발생할 수 있음
#endif

	glBegin(GL_LINE_LOOP);
		glVertex3f(50.0, 50.0, 0.0);
		glVertex3f(50.0, 30.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(50.0, 50.0, 0.0);
		glVertex3f(55.0, 50.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(75.0, 50.0, 0.0);
		glVertex3f(80.0, 50.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(80.0, 50.0, 0.0);
		glVertex3f(80.0, 30.0, 0.0);
	glEnd();

	glColor3f(1, 1, 0);

	glBegin(GL_LINE_LOOP);
		glVertex3f(55.0, 55.0, 0.0);
		glVertex3f(55.0, 45.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(55.0, 55.0, 0.0);
		glVertex3f(75.0, 55.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(75.0, 55.0, 0.0);
		glVertex3f(75.0, 45.0, 0.0);
	glEnd();

	glBegin(GL_LINE_LOOP);
		glVertex3f(55.0, 45.0, 0.0);
		glVertex3f(75.0, 45.0, 0.0);
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

	draw_resistance();

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
