#include <GL/glut.h>

// angle(각도)변수의 증가값으로 사용하는데 1.0 으로 초기화합니다.
float deltaAngle = 1.0;
float angleX = 1.0;
float angle = 1.0;

int specialKey;

int width = 320;
int height = 320;

float red, green, blue;

void changeSize(int w, int h)
{
    //창이 아주 작을 때, 0 으로 나누는 것을 예방합니다.
    if(h == 0)
        h = 1;
    float ratio = 1.0* w / h;

    //좌표계를 수정하기 전에 초기화합니다.
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //뷰포트를 창의 전체 크기로 설정합니다.
    glViewport(0, 0, w, h);

    //투시값을 설정합니다.
    gluPerspective(45,ratio,1,1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0,0.0,5.0, 0.0,0.0,-1.0, 0.0f,1.0f,0.0f);
}

void renderScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
        glRotatef(angle,0.0,1.0,0.0);
        glRotatef(angleX,1.0,0.0,0.0);
        glColor3f(0.5,0.2,0.7);

        glBegin(GL_TRIANGLES);
            glVertex3f(-0.5,-0.5,0.0);
            glVertex3f(0.5,0.0,0.0);
            glVertex3f(0.0,0.5,0.0);
        glEnd();
    glPopMatrix();
    // 새로운 코드입니다.
    // 이전 함수에서는 angle++ 이었습니다.
    angle+=deltaAngle;
    glutSwapBuffers();
}

void processMouse(int button, int state, int x, int y)
{
    specialKey = glutGetModifiers();

    // 마우스 버튼과 ALT 키가 같이 눌리면~
    if ((state == GLUT_DOWN) && (specialKey == GLUT_ACTIVE_ALT))
    {
        // 왼쪽 버튼을 눌렀으면 삼각형을 빨간색으로 설정합니다.
        if (button == GLUT_LEFT_BUTTON)
        {
            float red = 1.0, green = 0.0, blue = 0.0;
        }
        // 가운데 버튼을 눌렀으면 삼각형을 녹색으로 설정합니다.
        else if (button == GLUT_MIDDLE_BUTTON)
        {
            float red = 0.0, green = 1.0, blue = 0.0;
        }
        // 오른쪽 버튼을 눌렀으면 삼각형을 파란색으로 설정합니다.
        else
        {
            float red = 0.0, green = 0.0, blue = 1.0;
        }
    }
}

void processMousePassiveMotion(int x, int y)
{
    // X 축 중심으로 회전하게 만들려면 SHIFT 키를 눌러야합니다.
    if (specialKey != GLUT_ACTIVE_SHIFT)
    {
        // 마우스 위치에 따라서 회전 각도를 설정합니다.
        if (x < 0)
            angleX = 0.0;
        else if (x > width)
            angleX = 180.0;
        else
            angleX = 180.0 * ((float) x)/height;
    }
}

void processMouseActiveMotion(int x, int y)
{
    // ALT 키는 위 함수에서 검사합니다.
    if (specialKey != GLUT_ACTIVE_ALT)
    {
        // 마우스가 창의 내부에 있을 때
        // 마우스 위치에 따라 red 변수를 설정합니다.
        if (x < 0)
            red = 0.0;
        else if (x > width)
            red = 1.0;
        else
            red = ((float) x)/height;

        // 마우스가 창의 내부에 있을 때
        // 마우스 위치에 따라 green 변수를 설정합니다.
        if (y < 0)
            green = 0.0;
        else if (y > width)
            green = 1.0;
        else
            green = ((float) y)/height;

        // blue 변수(파란색)은 0.0 으로 설정해서 파란색 요소를 제거합니다.
        blue = 0.0;
    }
}

void processMouseEntry(int state) 
{
    if (state == GLUT_LEFT)
        deltaAngle = 0.0;
    else
        deltaAngle = 1.0;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(320,320);
    glutCreateWindow("SnowMen");
    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(changeSize);

    // 마우스 처리 콜백함수들을 등록합니다.
    glutMouseFunc(processMouse);
    glutMotionFunc(processMouseActiveMotion);
    glutPassiveMotionFunc(processMousePassiveMotion);
    glutEntryFunc(processMouseEntry);

    glutMainLoop();

	return 0;
}
