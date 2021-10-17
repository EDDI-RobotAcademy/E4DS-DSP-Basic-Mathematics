#include <stdio.h>

#include <GL/glut.h>

void my_disp(void)
{
	// 새로운 창을 그리기 위해 기존 창을 정리함
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, 300, 300);
	// 0.0 ~ 1.0
	// 0 ~ 255
	// r, g, b
	// 현재는 칠할 내용 없으므로 빨강색 적용은 안됨
	glColor3f(1.0, 0.0, 0.0);
	glFlush();
}

void onMouse(int button, int state, int x, int y) {
	if(state != GLUT_DOWN)
		return;

	int window_width = glutGet(GLUT_WINDOW_WIDTH);
	int window_height = glutGet(GLUT_WINDOW_HEIGHT);

	GLbyte color[4];
	GLfloat depth;
	GLuint index;

	glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

	printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
			x, y, color[0], color[1], color[2], color[3], depth, index);
}

int main(int argc, char **argv)
{
	// GL(Graphic Library) Utility - OpenGL
	// 라이브러리 이름이 gl, glut
	glutInit(&argc, argv);
	// RGBA를 표현할 수 있는 디스플레이 방식
	glutInitDisplayMode(GLUT_RGBA);
	// 창의 이름을 지정해줌
	glutCreateWindow("OGL Window Exam");
	// 실제로 화면에 보여질 내용을 콜백 처리함
	glutDisplayFunc(my_disp);
	// 실제 등록한 콜백을 루프 돌면서 처리하도록 만들어줌

	glutMouseFunc(onMouse);

	glutMainLoop();

	return 0;
}
