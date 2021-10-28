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

void gltDrawTorus(GLfloat majorRadius, GLfloat minorRadius,
		GLint numMajor, GLint numMinor)
{
	GLTVector3 vNormal;
	double majorStep = 2.0f*GLT_PI / numMajor;
	double minorStep = 2.0f*GLT_PI / numMinor;
	int i, j;
	for (i=0; i<numMajor; ++i)
	{
		double a0 = i * majorStep;
		double a1 = a0 + majorStep;
		GLfloat x0 = (GLfloat) cos(a0);
		GLfloat y0 = (GLfloat) sin(a0);
		GLfloat x1 = (GLfloat) cos(a1);
		GLfloat y1 = (GLfloat) sin(a1);
		glBegin(GL_TRIANGLE_STRIP);
		for (j=0; j<=numMinor; ++j)
		{
			double b = j * minorStep;
			GLfloat c = (GLfloat) cos(b);
			GLfloat r = minorRadius * c + majorRadius;
			GLfloat z = minorRadius * (GLfloat) sin(b);
			glTexCoord2f((float)(i)/(float)(numMajor), (float)(j)/(float)(numMinor));
			vNormal[0] = x0*c;
			vNormal[1] = y0*c;
			vNormal[2] = z/minorRadius;
			gltNormalizeVector(vNormal);
			glNormal3fv(vNormal);
			glVertex3f(x0*r, y0*r, z);
			glTexCoord2f((float)(i+1)/(float)(numMajor), (float)(j)/(float)(numMinor));
			vNormal[0] = x1*c;
			vNormal[1] = y1*c;
			vNormal[2] = z/minorRadius;
			glNormal3fv(vNormal);
			glVertex3f(x1*r, y1*r, z);
		}
		glEnd();
	}
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

void render_scene(void)
{
	int i;
	static GLfloat yRot = 0.0f;
	yRot += 0.5f;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	DrawGround();

	gluLookAt(  wx, 1.0f, wz,
            wx+lx, 1.0f,  wz+lz,
            0.0f, 1.0f,  0.0f);

#if 0
	for(i = 0; i < NUM_SPHERES; i++)
	{
		glPushMatrix();
		gltApplyActorTransform(&spheres[i]);
		glutSolidSphere(0.1f, 13, 26);
		glPopMatrix();
	}
#endif

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -2.5f);

#if 0
	glPushMatrix();
	glRotatef(-yRot * 2.0f, 0.0f, 1.0f, 0.0f);
	glTranslatef(1.0f, 0.0f, 0.0f);
	glutSolidSphere(0.1f, 13, 26);
	glPopMatrix();
#endif

#if 0
	glRotatef(yRot, 0.0f, 1.0f, 0.0f);
#endif

	gltDrawTorus(0.35, 0.15, 40, 20);
	glPopMatrix();

	glPopMatrix();
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
	glutTimerFunc(33, TimerFunction, 1);

	glutMainLoop();

	return 0;
}
