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
	glutMainLoop();

	return 0;
}
