#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>

void draw_y_x2(void);
void draw_big_rect(void);
void draw_small_rect(void);

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

	// 선의 굵기를 조정할 수 있음
	glLineWidth(3);
	// y = sin(x) 그리기 
	draw_y_x2();
	glLineWidth(1);
	glColor3f(0, 0, 1);
	//draw_small_rect();
	glColor3f(0, 1, 0);
	draw_big_rect();
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

void draw_y_x2(void)
{
	float t = -100.0, step = 0.001;
	float x = 0, x2 = 0, y2, cx, cy;
	float tmp;
	int cache = 0;

	if(t > 100)
		t = 0.0;

	glBegin(GL_LINES);
	// 여기서 미소값을 결정하는 부분은 step입니다.
	// step을 작게 만들면 그래프가 보다 정확해지고
	// 이를 크게 만들면 손실하게 되는 정보가 발생합니다(샘플링 이론)
	for(; ; t += step)
	{
		if(t > 100)
			break;

		// A * sin(w * t)
		// A * sin(2 * pi * f * t)
		// w = 2pi * f
		// f = w / 2pi = (25 / 100) * (1 / 6.28)
		y2 = 50 * sin(0.25 * t);

		if(cache)
		{
			// 이전값
			glVertex2f(cx, cy);
			// 현재값
			glVertex2f(t, y2);
			// 이전 (x, y) 점을 현재 (x, y)점과 연결 시켜서 선분을 만듬
		}

		cache = 1;
		cx = t;
		cy = y2;
	}
	glEnd();
}

void draw_big_rect(void)
{
	float t = 0.0, step = 2.5;
	float x2 = 0, y2, cx, cy;
	int cache = 0;

	if(t > 100)
		t = 0.0;

	for(; ; t += step)
	{
		if(t > 100)
			break;

		y2 = 50 * sin(0.25 * t);

		if(cache)
		{

			glBegin(GL_LINES);
				glVertex2f(cx, 0);
				glVertex2f(t, 0);
			glEnd();

			glBegin(GL_LINES);
				glVertex2f(cx, 0);
				glVertex2f(cx, y2);
			glEnd();

			glBegin(GL_LINES);
				glVertex2f(cx, y2);
				glVertex2f(t, y2);
			glEnd();

			glBegin(GL_LINES);
				glVertex2f(t, y2);
				glVertex2f(t, y2);
			glEnd();

			if(cy > y2) {
				printf("t = %f, y2 = %f\n", t, y2);
				printf("cx = %f, cy = %f\n", cx, cy);

				glBegin(GL_LINES);
					glVertex2f(cx, cy);
					glVertex2f(cx, 0);
				glEnd();
			}

			if(y2 < 0) {
                printf("t = %f, y2 = %f\n", t, y2);
                printf("cx = %f, cy = %f\n", cx, cy);

                glBegin(GL_LINES);
                    glVertex2f(t, y2);
                    glVertex2f(t, 0);
                glEnd();
            }
		}

		cache = 1;
		cx = t;
		cy = y2;
	}
}

void draw_small_rect(void)
{
	float t = 0.0, step = 0.0001;
	float x2 = 0, y2, cx, cy;
	int cache = 0;

	if(t > 100)
		t = 0.0;

	for(; ; t += step)
	{
		if(t > 100)
			break;

		y2 = 50 * sin(0.25 * t);

		if(cache)
		{
			// 사각형을 그리기 위해 y값은 0으로 고정시키고 x의 변화를 그림
			// 밑변 그리기
			glBegin(GL_LINES);
				glVertex2f(cx, 0);
				glVertex2f(t, 0);
			glEnd();

			// x(cx)를 고정시키고 y를 0 ~ y2로 즉 높이값에 해당하는 y를 그림
			glBegin(GL_LINES);
				glVertex2f(cx, 0);
				glVertex2f(cx, y2);
			glEnd();

			// y 높이가 고정되고 x를 cx ~ t로 그림(윗변)
			glBegin(GL_LINES);
				glVertex2f(cx, y2);
				glVertex2f(t, y2);
			glEnd();

			// x(t)를 고정 시키고 y
			// 실제로는 맨 마지막에 1번만 배치하는 것이 성능상에는 더 좋음
			glBegin(GL_LINES);
				glVertex2f(t, y2);
				glVertex2f(t, y2);
			glEnd();
		}

		cache = 1;
		cx = t;
		cy = y2;
	}
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
