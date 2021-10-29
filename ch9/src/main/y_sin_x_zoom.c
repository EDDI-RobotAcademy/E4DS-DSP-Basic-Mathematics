#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/glu.h>

float angle=0.0;
// 카메라 방향
float lx=0.0f,lz=-1.0f;
// 카메라 xz 위치
float wx=0.0f,wz=5.0f;

// 실제 OpenGL에서 x, z가 평면을 구성하고
// 높이는 y가 처리합니다.
#define SLICE	360

typedef GLfloat GLTMatrix[16];
typedef GLfloat GLTVector3[3];

typedef struct{
	GLTVector3 vLocation;
	GLTVector3 vUp;
	GLTVector3 vForward;
} GLTFrame;

#define NUM_SPHERES 50

GLTFrame spheres[NUM_SPHERES];
GLTFrame frameCamera;

#define GLT_PI				3.14159265358979323846
#define GLT_PI_DIV_180		0.017453292519943296
#define gltDegToRad(x)		((x)*GLT_PI_DIV_180)

void gltScaleVector(GLTVector3 vVector, const GLfloat fScale)
{
	vVector[0] *= fScale; vVector[1] *= fScale; vVector[2] *= fScale;
}

GLfloat gltGetVectorLengthSqrd(const GLTVector3 vVector)
{
	return (vVector[0]*vVector[0]) + (vVector[1]*vVector[1]) + (vVector[2]*vVector[2]);
}

GLfloat gltGetVectorLength(const GLTVector3 vVector)
{
	return (GLfloat)sqrt(gltGetVectorLengthSqrd(vVector));
}

void gltNormalizeVector(GLTVector3 vNormal)
{
	GLfloat fLength = 1.0f / gltGetVectorLength(vNormal);
	gltScaleVector(vNormal, fLength);
}

void gltInitFrame(GLTFrame *pFrame)
{
	pFrame->vLocation[0] = 0.0f;
	pFrame->vLocation[1] = 0.0f;
	pFrame->vLocation[2] = 0.0f;
	pFrame->vUp[0] = 0.0f;
	pFrame->vUp[1] = 1.0f;
	pFrame->vUp[2] = 0.0f;
	pFrame->vForward[0] = 0.0f;
	pFrame->vForward[1] = 0.0f;
	pFrame->vForward[2] = -1.0f;
}

void gltVectorCrossProduct(const GLTVector3 vU,
		const GLTVector3 vV, GLTVector3 vResult)
{
	vResult[0] = vU[1]*vV[2] - vV[1]*vU[2];
	vResult[1] = -vU[0]*vV[2] + vV[0]*vU[2];
	vResult[2] = vU[0]*vV[1] - vV[0]*vU[1];
}

void gltGetMatrixFromFrame(GLTFrame *pFrame, GLTMatrix mMatrix)
{
	GLTVector3 vXAxis;
	gltVectorCrossProduct(pFrame->vUp, pFrame->vForward, vXAxis);
	memcpy(mMatrix, vXAxis, sizeof(GLTVector3));
	mMatrix[3] = 0.0f;
	memcpy(mMatrix+4, pFrame->vUp, sizeof(GLTVector3));
	mMatrix[7] = 0.0f;
	memcpy(mMatrix+8, pFrame->vForward, sizeof(GLTVector3));
	mMatrix[11] = 0.0f;
	memcpy(mMatrix+12, pFrame->vLocation, sizeof(GLTVector3));
	mMatrix[15] = 1.0f;
}

void gltApplyActorTransform(GLTFrame *pFrame)
{
	GLTMatrix mTransform;
	gltGetMatrixFromFrame(pFrame, mTransform);
	glMultMatrixf(mTransform);
}

void SetupRC()
{
	int iSphere;
	glClearColor(0.0f, 0.0f, .50f, 1.0f );
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	gltInitFrame(&frameCamera);
	for(iSphere = 0; iSphere < NUM_SPHERES; iSphere++)
	{
		gltInitFrame(&spheres[iSphere]);
		spheres[iSphere].vLocation[0] = (float)((rand() % 400) - 200) * 0.1f;
		spheres[iSphere].vLocation[1] = 0.0f;
		spheres[iSphere].vLocation[2] = (float)((rand() % 400) - 200) * 0.1f;
	}
}

void DrawGround(void)
{
	GLfloat fExtent = 20.0f;
	GLfloat fStep = 1.0f;
	GLfloat y = -0.4f;
	GLint iLine;

	glColor3f(1, 1, 1);
	glBegin(GL_LINES);
	for(iLine = -fExtent; iLine <= fExtent; iLine += fStep)
	{
		glVertex3f(iLine, y, fExtent);
		glVertex3f(iLine, y, -fExtent);
		glVertex3f(fExtent, y, iLine);
		glVertex3f(-fExtent, y, iLine);
	}
	glEnd();
}

void calc_period(float *freq, float *period)
{
    *period = 1 / (*freq);
}

void calc_angular_velocity(float *freq, float *ang_vel)
{
    *ang_vel = 2 * M_PI * (*freq);
}

float get_step(float slice, float period)
{
    return period / slice;
}

void draw_y_sinx(void)
{
    float amp, period, freq, omega, t, step = 0.0;
    float x2 = 0, y2, cx, cy;
    int cache = 0;

    amp = 10;
    freq = 100.0;

    calc_period(&freq, &period);
    calc_angular_velocity(&freq, &omega);

    t = step = get_step(SLICE, period);

    if(t > period)
        t = 0.0;

	glColor3f(0, 1, 1);
	glBegin(GL_LINES);
    for(; ; t += step)
    {
        if(t > 3 * period)
        {
            break;
            t = 0.0;
        }

        y2 = amp * sin(omega * t);

        if(cache)
        {
#if 0
            glVertex3f(cx * 4000, cy * 100, 0);
            glVertex3f(t * 4000, y2 * 100, 0);
#endif
#if 0
            glVertex3f(0, cx * 4000, cy * 100);
            glVertex3f(0, t * 4000, y2 * 100);
#endif
            glVertex3f(cx * 400, cy, -50.0 + wz);
            glVertex3f(t * 400, y2, -50.0 + wz);
        }

        cache = 1;
        cx = t;
        cy = y2;
        printf("t = %f, y2 = %f, wz = %f\n", t * 400, y2, wz);
    }
    glEnd();

	glColor3f(1, 0, 0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, -50.0);
		glVertex3f(0.0, 100.0, -50.0);
	glEnd();

	glColor3f(0, 1, 0);
	glBegin(GL_LINE_LOOP);
		glVertex3f(0.0, 0.0, -50.0);
		glVertex3f(100.0, 0.0, -50.0);
	glEnd();
}

void render_scene(void)
{
	int i;
	static GLfloat yRot = 0.0f;
	yRot += 0.5f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	//DrawGround();
	gluLookAt(  wx, 1.0f, wz,
            wx+lx, 1.0f,  wz+lz,
            0.0f, 1.0f,  0.0f);

#if 0
	glPushMatrix();
	glPopMatrix();
#endif

	glPopMatrix();
	draw_y_sinx();

	glutSwapBuffers();
}

void TimerFunction(int value)
{
	glutPostRedisplay();
	glutTimerFunc(3,TimerFunction, 1);
}

void change_size(int w, int h)
{
	GLfloat fAspect;
	if(h == 0)
		h = 1;
	glViewport(0, 0, w, h);
	fAspect = (GLfloat)w / (GLfloat)h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0f, fAspect, 1.0f, 50.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void draw_resistance(void)
{
	glColor3f(1, 0, 1);

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

#if 0
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
#endif

void mouse(int button, int state, int x, int y)
{
	float fraction = 0.1f;

#if 0
   if ((button == 3) || (button == 4))
   {
       if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
       printf("Scroll %s At %d %d\n", (button == 3) ? "Up" : "Down", x, y);
#endif
	if (button == 3)	// 스크롤 업
	{
		printf("Up\n");
		wx += lx * fraction;
		wz += lz * fraction;
	}
	else if (button == 4)	// 다운
	{
		printf("Down\n");
		wx -= lx * fraction;
		wz -= lz * fraction;
   }else{  // normal button event
       printf("Button %s At %d %d\n", (state == GLUT_DOWN) ? "Down" : "Up", x, y);
   }

	glutPostRedisplay();
   printf("x = %f, z = %f\n", wx, wz);
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Digital Signal Processing");

	glutDisplayFunc(render_scene);
	glutReshapeFunc(change_size);

	//glutMouseWheelFunc(wheel_zoom_in_out);
	glutMouseFunc(mouse);

	SetupRC();
	//glutTimerFunc(33, TimerFunction, 1);

	glutMainLoop();

	return 0;
}
