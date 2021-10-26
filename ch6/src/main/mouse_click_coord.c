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

// glutMouseFunc() 여기서 동작하게 되면
// 자동으로 마우스 클릭한 위치의 x, y값이 콜백으로 동작하는 함수에 설정됨
// 그래서 onMouse에서 별도의 함수 호출 없이도 x, y값을 확보할 수 있습니다.
void onMouse(int button, int state, int x, int y) {
	// GLUT_DOWN(마우스 클릭 했나요 ?)
	// 마우스 클릭을 하지 않았다면 그냥 함수 종료
	// 마우스 휠도 GLUT_DOWN으로 처리되므로 주의가 필요합니다!
	if(state != GLUT_DOWN)
		return;

	// 현재 띄워진 창의 width(폭)과 height(높이) 값을 구해옵니다.
	//int window_width = glutGet(GLUT_WINDOW_WIDTH);
	//int window_height = glutGet(GLUT_WINDOW_HEIGHT);

	//GLbyte color[4];
	//GLfloat depth;
	//GLuint index;

	//glReadPixels(x, window_height - y - 1, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, color);
	//glReadPixels(x, window_height - y - 1, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depth);
	//glReadPixels(x, window_height - y - 1, 1, 1, GL_STENCIL_INDEX, GL_UNSIGNED_INT, &index);

#if 0
	printf("Clicked on pixel %d, %d, color %02hhx%02hhx%02hhx%02hhx, depth %f, stencil index %u\n",
			x, y, color[0], color[1], color[2], color[3], depth, index);
#endif
	printf("Clicked on pixel %d, %d, \n", x, y);
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
