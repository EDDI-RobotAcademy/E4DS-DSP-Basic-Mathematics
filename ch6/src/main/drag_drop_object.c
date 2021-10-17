#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <time.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>

#include <unistd.h>
#include <signal.h>

//#include <SDL2/SDL.h>

#define SLICE	360

typedef struct _line_vertex line_vertex;
struct _line_vertex
{
	float x1;
	float y1;

	float x2;
	float y2;
};

enum btn_state
{
    ENone,
    EPressed,
    EReleased,
    EHeld
};

float text_x = 64.0f, text_y = 58.0f;
const float TRANS_VAL = 100.0f;

uint32_t mouse_loc;
int x, y;

bool clicked = false;
int pos_x, pos_y;
int mov_x, mov_y;
int click_x, click_y;
int special_key;

line_vertex vertices[9] = {
	{ 50.0f, 50.0f, 50.0f, 30.0f },
	{ 50.0f, 50.0f, 55.0f, 50.0f },
	{ 75.0f, 50.0f, 80.0f, 50.0f },
	{ 80.0f, 50.0f, 80.0f, 30.0f },
	{ 55.0f, 55.0f, 55.0f, 45.0f },
	{ 55.0f, 55.0f, 75.0f, 55.0f },
	{ 75.0f, 55.0f, 75.0f, 45.0f },
	{ 55.0f, 45.0f, 75.0f, 45.0f }
};

line_vertex pick_vertices[9] = {
	{ 45.0f, 25.0f, 45.0f, 65.0f },
	{ 45.0f, 25.0f, 85.0f, 25.0f },
	{ 45.0f, 65.0f, 85.0f, 65.0f },
	{ 85.0f, 25.0f, 85.0f, 65.0f }
};

void timer_handler(int signo)
{
	int i;

	for (i = 0; i < 8; i++)
	{
		vertices[i].x1 -= TRANS_VAL;
		vertices[i].x2 -= TRANS_VAL;
	}

	text_x -= TRANS_VAL;

	glutPostRedisplay();
}

#if 0
bool get_button_value(int button)
{
    return (SDL_BUTTON(button) & m_curr_btns) == 1;
}

btn_state get_button_state(int button)
{
    int mask = SDL_BUTTON(button);
    if ((mask & m_prev_btns) == 0)
    {
        if ((mask & m_curr_btns) == 0)
        {
            return ENone;
        }
        else
        {
            return EPressed;
        }
    }
    else
    {
        if ((mask & m_curr_btns) == 0)
        {
            return EReleased;
        }
        else
        {
            return EHeld;
        }
    }
}
#endif

void on_mouse(int button, int state, int x, int y)
{
	special_key = glutGetModifiers();

#if 0
	if (state != GLUT_DOWN)
		return;
#endif

	if (button == GLUT_LEFT_BUTTON & state == GLUT_DOWN)
	{
		if (clicked == false)
		{
			printf("Click - Down\n");
			click_x = x;
			click_y = y;
			clicked = true;

			printf("click_x = %d, click_y = %d\n", click_x, click_y);
		}
	}

	if (button == GLUT_LEFT_BUTTON & state == GLUT_UP)
	{
		printf("Click - Up\n");
		clicked = false;
	}

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

void drag_mouse(int x, int y)
{
	pos_x = x;
	pos_y = y;

	special_key = glutGetModifiers();

	if (clicked == true)
	{
#if 0
		if (special_key == GLUT_ACTIVE_SHIFT)
		{
#endif
			printf("clicked!\n");

			mov_x = (click_x - pos_x) / 3;
			mov_y = (click_y - pos_y) / 3;

			click_x = pos_x;
			click_y = pos_y;

			printf("mov_x = %d, mov_y = %d\n", mov_x, mov_y);

			int i;

			for (i = 0; i < 8; i++)
			{ 
				vertices[i].x1 -= mov_x;
				vertices[i].x2 -= mov_x;
				vertices[i].y1 += mov_y;
				vertices[i].y2 += mov_y;
			}

			for (i = 0; i < 4; i++)
			{ 
				pick_vertices[i].x1 -= mov_x;
				pick_vertices[i].x2 -= mov_x;
				pick_vertices[i].y1 += mov_y;
				pick_vertices[i].y2 += mov_y;
			}

			text_x -= mov_x;
			text_y += mov_y;

			glutPostRedisplay();
#if 0
		}
#endif
	}
}

void drawString (char *s)
{
	unsigned int i;

	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_10, s[i]);
}

void drawStringBig (char *s)
{
	unsigned int i;

	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_18, s[i]);
}

void draw_outline(void)
{
	int i;

	glColor3f(0, 1, 0);

	glBegin(GL_LINES);
		for (i = 0; i < 8; i++)
		{
			glVertex2f(pick_vertices[i].x1, pick_vertices[i].y1);
			glVertex2f(pick_vertices[i].x2, pick_vertices[i].y2);
		}
	glEnd();
}

void draw_resistance(void)
{
	int i;
	static char label[100];

	glColor3f(1, 0, 1);

	glBegin(GL_LINES);
		for (i = 0; i < 8; i++)
		{
			glVertex2f(vertices[i].x1, vertices[i].y1);
			glVertex2f(vertices[i].x2, vertices[i].y2);
		}
	glEnd();

	sprintf (label, "R");
	glRasterPos2f (text_x, text_y);
	drawStringBig (label);
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
	draw_outline();

	//mouse_loc = SDL_GetMouseState(&x, &y);
	//printf("mouse_loc = %u, x = %u, y = %u\n", mouse_loc, x, y);

	glFlush();
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

#if 0
    if(w <= h)
        glOrtho(-n_range, n_range, -n_range * h / w, n_range * h / w, -n_range, n_range);
    else
        glOrtho(-n_range * w / h, n_range * w / h, -n_range, n_range, -n_range, n_range);
#endif
	glOrtho(-100, 100, -100, 100, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Digital Signal Processing");

	//m_curr_state = SDL_GetKeyboardState(NULL);

	//signal(SIGALRM, timer_handler);
	//alarm(5);

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(on_mouse);
	glutMotionFunc(drag_mouse);
	glutMainLoop();

	return 0;
}
