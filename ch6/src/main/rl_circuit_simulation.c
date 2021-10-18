#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>
#include <gsl/gsl_odeiv2.h>

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <time.h>
#include <math.h>

#include <GL/glut.h>
#include <GL/gl.h>

#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

#define SLICE	360

typedef struct _line_vertex line_vertex;
struct _line_vertex
{
	float x1;
	float y1;

	float x2;
	float y2;
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

bool run = false;

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

line_vertex power_vertices[5] = {
	{ 40.0f, 50.0f, 40.0f, 60.0f },
	{ 50.0f, 50.0f, 30.0f, 50.0f },
	{ 45.0f, 45.0f, 35.0f, 45.0f },
	{ 40.0f, 45.0f, 40.0f, 35.0f }
};

line_vertex ground_vertices[5] = {
	{ 40.0f, 50.0f, 40.0f, 60.0f },
	{ 50.0f, 50.0f, 30.0f, 50.0f },
	{ 47.5f, 47.5f, 32.5f, 47.5f },
	{ 45.0f, 45.0f, 35.0f, 45.0f }
};

int *jac;

int rhs (double t, const double y[], double f[], void *params_ptr);
//int jacobian (double t, const double y[], double *dfdy, double dfdt[], void *params_ptr);

void calc_differential_eq(void);
void draw_differential_eq_plot(void);
void draw_lattice(void);
void draw_lattice2(void);
void draw_volt_plot(void);

void reshape(int w, int h);

typedef struct _xy_plot xy_plot;
struct _xy_plot
{
	double x;
	double y;
};

//xy_plot plot_res[1001];
xy_plot plot_res[100000001];
//double volt_res[100000001];

float resistance;
float inductance;
float voltage;

int sub_window;
int sub_window2;

void sim_reshape(int w, int h)
{
    GLfloat n_range = 100.0f;

    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glOrtho(-0.000005, 0.000005, -0.7, 0.7, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void simulate(void)
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

    draw_differential_eq_plot();
    draw_lattice();

    glutSwapBuffers();
}

void sim_reshape2(int w, int h)
{
    GLfloat n_range = 100.0f;

    if(h == 0)
        h = 1;

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

	glOrtho(-0.000005, 0.000005, -2000000, 500000, -1, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void simulate2(void)
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
        glVertex3f(0.0, 500000.0, 0.0);
        glVertex3f(0.0, -2000000.0, 0.0);
    glEnd();

    draw_volt_plot();
    draw_lattice2();

    glutSwapBuffers();
}

void keyboard_handler(unsigned char key, int x, int y)
{
	int pid;
	int i, status;

	char *argv[5] = {"print_simulation"};
	char *envp[] = {0};
	char volt[64] = {0};
	char coil[64] = {0};
	char resist[64] = {0};

	switch(key)
	{
		case 's':
			printf("Simulation Preparation\n");
			printf("저항값 입력: ");
			scanf("%f", &resistance);
			printf("인덕턴스값 입력: ");
			scanf("%f", &inductance);
			printf("입력 전원값 입력: ");
			scanf("%f", &voltage);

			printf("R = %f, L = %f, V = %f\n", resistance, inductance, voltage);

			calc_differential_eq();
			run = true;
			break;
		case 'r':
			printf("Run Simulation\n");

#if 0
			pid = fork();

			if (pid > 0)
			{
				printf("wait for simulation finished\n");
				wait(&status);
			}
			else if (pid == 0)
			{
				execve("./print_simulation", argv, envp);
			}
			else
			{
				perror("fork() ");
				exit(-1);
			}
#endif

			sub_window = glutCreateWindow("Current i(t)");

			glutDisplayFunc(simulate);
			glutReshapeFunc(sim_reshape);

			sub_window2 = glutCreateWindow("Voltage V(t)");

			glutDisplayFunc(simulate2);
			glutReshapeFunc(sim_reshape2);
			break;
		case 't':
			printf("Termination\n");
			glutDestroyWindow(sub_window);
			glutDestroyWindow(sub_window2);
			break;
		case 27:
			exit(1);
			break;
	}
}

void on_mouse(int button, int state, int x, int y)
{
	special_key = glutGetModifiers();

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
		printf("clicked!\n");

		mov_x = (click_x - pos_x) / 3;
		mov_y = (click_y - pos_y) / 2.5;

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

		for (i = 0; i < 4; i++)
		{
			power_vertices[i].x1 -= mov_x;
			power_vertices[i].x2 -= mov_x;
			power_vertices[i].y1 += mov_y;
			power_vertices[i].y2 += mov_y;
		}

		for (i = 0; i < 4; i++)
		{
			ground_vertices[i].x1 -= mov_x;
			ground_vertices[i].x2 -= mov_x;
			ground_vertices[i].y1 += mov_y;
			ground_vertices[i].y2 += mov_y;
		}

		text_x -= mov_x;
		text_y += mov_y;

		glutPostRedisplay();
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

void draw_inductance_outline(void)
{
	int i;

	glColor3f(0, 1, 0);

	glBegin(GL_LINES);
		for (i = 0; i < 4; i++)
		{
			glVertex2f(pick_vertices[i].x1 + 50.0f, pick_vertices[i].y1);
			glVertex2f(pick_vertices[i].x2 + 50.0f, pick_vertices[i].y2);
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

void draw_inductance(void)
{
	int i;
	static char label[100];

	glColor3f(1, 0, 1);

	glBegin(GL_LINES);
		for (i = 0; i < 8; i++)
		{
			glVertex2f(vertices[i].x1 + 50.0f, vertices[i].y1);
			glVertex2f(vertices[i].x2 + 50.0f, vertices[i].y2);
		}
	glEnd();

	sprintf (label, "L");
	glRasterPos2f (text_x + 50.0f, text_y);
	drawStringBig (label);
}

void draw_power_outline(void)
{
	int i;

	glColor3f(0, 1, 0);

	glBegin(GL_LINES);
		for (i = 0; i < 4; i++)
		{
			glVertex2f(pick_vertices[i].x1 - 75.0f, pick_vertices[i].y1 + 20.0f);
			glVertex2f(pick_vertices[i].x2 - 75.0f, pick_vertices[i].y2 + 20.0f);
		}
	glEnd();
}

void draw_power(void)
{
	int i;
	static char label[100];

	glColor3f(1, 0, 1);

	glBegin(GL_LINES);
		for (i = 0; i < 4; i++)
		{
			glVertex2f(power_vertices[i].x1 - 50.0f, power_vertices[i].y1 + 15.0f);
			glVertex2f(power_vertices[i].x2 - 50.0f, power_vertices[i].y2 + 15.0f);
		}
	glEnd();

	sprintf (label, "V");
	glRasterPos2f (text_x - 70.0f, text_y + 20.0f);
	drawStringBig (label);
}

void draw_ground_outline(void)
{
	int i;

	glColor3f(0, 1, 0);

	glBegin(GL_LINES);
		for (i = 0; i < 4; i++)
		{
			glVertex2f(pick_vertices[i].x1 - 75.0f, pick_vertices[i].y1 - 20.0f);
			glVertex2f(pick_vertices[i].x2 - 75.0f, pick_vertices[i].y2 - 20.0f);
		}
	glEnd();
}

void draw_ground(void)
{
	int i;
	static char label[100];

	glColor3f(1, 0, 1);

	glBegin(GL_LINES);
		for (i = 0; i < 4; i++)
		{
			glVertex2f(ground_vertices[i].x1 - 50.0f, ground_vertices[i].y1 - 30.0f);
			glVertex2f(ground_vertices[i].x2 - 50.0f, ground_vertices[i].y2 - 30.0f);
		}
	glEnd();

	sprintf (label, "GND");
	glRasterPos2f (text_x - 70.0f, text_y - 30.0f);
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

	draw_inductance();
	draw_inductance_outline();

	draw_power();
	draw_power_outline();

	draw_ground();
	draw_ground_outline();

#if 0
	if (run)
	{
		draw_differential_eq_plot();
	}

	draw_lattice();
#endif

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

	//glOrtho(-10, 10, -0.5, 0.5, -1, 1);
	glOrtho(-100, 100, -100, 100, 1.0, -1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int calc_vertices_num(void)
{
	double tmin = 0.0, tmax = 10.0, delta_t = 0.01;
	return (tmax - tmin) / delta_t;
}

void calc_differential_eq(void)
{
    int dim = 1;

	gsl_odeiv2_system sys = {rhs, NULL, dim, NULL};  
	gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rkf45, 1e-6, 1e-6, 0.0);

	int cnt = 0;

	double i;
    double x0 = 0.0,  xf = 10.0;
    double x = x0;
    double y[1] = { 0.5 };
	double delta = 0.0000001;

	for (i = 0; i <= xf; i += delta)
    {
        double xi = x0 + i * (xf-x0) / xf;
        int status = gsl_odeiv2_driver_apply (d, &x, xi, y);

        if (status != GSL_SUCCESS)
        {
            printf ("error, return value=%d\n", status);
            break;
        }

		plot_res[cnt].x = x;
		plot_res[cnt].y = y[0];

		//volt_res[cnt] = y[0] * (resistance / inductance);

		if (cnt < 1001)
		{
       		printf ("%.8e %.8e\n", plot_res[cnt].x, plot_res[cnt].y);
		}

		cnt++;
    }

	printf("Finish Calc\n");

    gsl_odeiv2_driver_free (d);

#if 0
    const gsl_odeiv_step_type *type_ptr = gsl_odeiv_step_rkf45;

    gsl_odeiv_step *step_ptr = gsl_odeiv_step_alloc (type_ptr, dimension);
    gsl_odeiv_control *control_ptr = gsl_odeiv_control_y_new (eps_abs, eps_rel);
    gsl_odeiv_evolve *evolve_ptr = gsl_odeiv_evolve_alloc (dimension);

    gsl_odeiv_system my_system;	/* structure with the rhs function, etc. */

    double mu = 10;		/* parameter for the diffeq */
    double y[2];			/* current solution vector */

    double t, t_next;		/* current and next independent variable */
    double tmin, tmax, delta_t;	/* range of t and step size for output */

    double h = 1e-6;		/* starting step size for ode solver */

	int cnt = 0;
	int vertex_num = calc_vertices_num();

    my_system.function = rhs;	/* the right-hand-side functions dy[i]/dt */
    my_system.jacobian = NULL;
    my_system.dimension = dimension;	/* number of diffeq's */
    my_system.params = NULL;

    tmin = 0.;			/* starting t value */
    tmax = 10.;			/* final t value */
    delta_t = 0.01;

    y[1] = 0.5;			/* initial x value */

    t = tmin;             /* initialize t */

    for (t_next = tmin + delta_t; t_next <= tmax; t_next += delta_t)
    {
        while (t < t_next)	/* evolve from t to t_next */
        {
            gsl_odeiv_evolve_apply (evolve_ptr, control_ptr, step_ptr,
                                    &my_system, &t, t_next, &h, y);
        }

		plot_res[cnt].x = t;
		plot_res[cnt].y = y[0];
        printf ("%.5e %.5e %.5e\n", t, plot_res[cnt].x, plot_res[cnt].y); /* print at t=t_next */

		cnt++;
    }

    gsl_odeiv_evolve_free (evolve_ptr);
    gsl_odeiv_control_free (control_ptr);
    gsl_odeiv_step_free (step_ptr);
#endif
}

void draw_lattice(void)
{
	int i;
	float lattice = 0.000001;
	char buf[64] = {0};

	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
		for (i = 1; i <= 10; i++)
		{
			glVertex2f(i * 0.000001, -0.01);
			glVertex2f(i * 0.000001, 0.01);
		}
	glEnd();

	for (i = 1; i <= 10; i++)
	{
		sprintf(buf, "%f", lattice * i);
	    glRasterPos2f (lattice * i - 0.0000005, -0.05);
	    drawStringBig (buf);
	}

	glBegin(GL_LINES);
		for (i = 1; i <= 10; i++)
		{
			glVertex2f(-0.0000002, 0.1 * i);
			glVertex2f(0.0000002, 0.1 * i);
		}
	glEnd();

	for (i = 1; i <= 10; i++)
	{
		sprintf(buf, "%.1f", 0.1 * i);
	    glRasterPos2f (-0.0000004, 0.1 * i - 0.05);
	    drawStringBig (buf);
	}
}

void draw_differential_eq_plot(void)
{
	static char label[100];

	float x = 0, x2 = 0, y2, cx, cy;
	float tmp;
	int cache = 0;
	int i;

	glColor3f(0, 1, 1);
	glBegin(GL_LINES);
	//for(; ; t += step)
	for (i = 0; i < 100000000; i++)
	{
		x2 = plot_res[i].x;
		y2 = plot_res[i].y;

		if(cache)
		{
			glVertex2f(cx, cy);	// 이전값
			glVertex2f(x2, y2);	// 현재값
		}

		cache = 1;
		cx = x2;
		cy = y2;
		//printf("t = %f, y2 = %f\n", x2, y2);
	}
	glEnd();

	sprintf (label, "i(t)");
	// -0.000005
    glRasterPos2f (-0.0000045, 0.0000045);
    drawStringBig (label);
}

void draw_lattice2(void)
{
	int i;
	float lattice = 0.000001;
	char buf[64] = {0};

	glColor3f(1, 0, 0);
	glBegin(GL_LINES);
		for (i = 1; i <= 10; i++)
		{
			glVertex2f(i * 0.000001, -20000);
			glVertex2f(i * 0.000001, 20000);
		}
	glEnd();

	for (i = 1; i <= 10; i++)
	{
		sprintf(buf, "%f", lattice * i);
	    glRasterPos2f (lattice * i - 0.0000005, 50000);
	    drawStringBig (buf);
	}

	glBegin(GL_LINES);
		for (i = 1; i <= 10; i++)
		{
			glVertex2f(-0.0000002, -200000 * i);
			glVertex2f(0.0000002, -200000 * i);
		}
	glEnd();

	for (i = 1; i <= 10; i++)
	{
		sprintf(buf, "%d", -200000 * i);
	    glRasterPos2f (-0.0000015, -200000 * i);
	    drawStringBig (buf);
	}

}

void draw_volt_plot(void)
{
	static char label[100];

	float x = 0, x2 = 0, y2, cx, cy;
	float tmp;
	int cache = 0;
	int i;

	glColor3f(1, 1, 0);
	glBegin(GL_LINES);
	//for(; ; t += step)
	for (i = 0; i < 100000000; i++)
	{
		x2 = plot_res[i].x;
		y2 = plot_res[i].y * (-resistance / inductance);

		if(cache)
		{
			glVertex2f(cx, cy);	// 이전값
			glVertex2f(x2, y2);	// 현재값
		}

		cache = 1;
		cx = x2;
		cy = y2;
		//printf("t = %f, y2 = %f\n", x2, y2);
	}
	glEnd();

	sprintf (label, "V(t)");
	// -0.000005
    glRasterPos2f (-0.0000045, 0.000004);
    drawStringBig (label);
}

int main (int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Digital Signal Processing");

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(on_mouse);
	glutMotionFunc(drag_mouse);
	glutKeyboardFunc(keyboard_handler);
	glutMainLoop();

    return 0;
}

/*************************** rhs ****************************/
/* 
   x' = v                  ==>  dy[0]/dt = f[0] = y[1]
   v' = -x + \mu v (1-x^2) ==>  dy[1]/dt = f[1] = -y[0] + mu*y[1]*(1-y[0]*y[0])
  
   x''(t) + x'(t) + x(t) = 0
   x''(t) = -x'(t) - x(t)

   x' = v            ===> dy[0]/dt = f[0] = y[1]
   v' = -v - x       ===> dy[1]/dt = f[1] = -y[1] - y[0]
*/

// https://www.wolframalpha.com/input/?i=y%27%27+%2B+y%27+%2B+y+%3D+0%2C+y%280%29+%3D+1%2C+y%27%280%29+%3D+0, y(0) = 1
// https://www.wolframalpha.com/input/?i=%28sqrt%283%29+*+sin%28sqrt%283%29+%2F+2%29+%2B+3+*+cos%28sqrt%283%29+%2F+2%29%29+%2F+%283+*+sqrt%28e%29%29, y(1)
// (sqrt(3) * sin(sqrt(3) / 2) + 3 * cos(sqrt(3) / 2)) / (3 * sqrt(e)), y(1) = 0.6597001...
int
rhs (double t, const double y[], double f[], void *params_ptr)
{
#if 0
	f[0] = y[1];
	f[1] = -y[1] - 9 * y[0];
#endif

	f[0] =  -(resistance / inductance) * y[0] + voltage / inductance;

    return GSL_SUCCESS;		/* GSL_SUCCESS defined in gsl/errno.h as 0 */
}

#if 0

V = iR + L * di /dt
초기조건 V_L(t) | (t=0) ===> V_L(0) = 0V

V * (1 / L) = i(t) * (R / L) + i`(t)

P(x) = R / L

mu = C * exp^(int P(x)dx)

mu = C * exp^(R/L)t

d (mu * i(t)) / dt = (R/L) exp^(R/L)t * i(t) + exp^(R/L)t * i`(t)

int d(mu * i(t)) / dt = mu * i(t) = c

exp^(R/L)t * i(t) = C

i(t) = C * exp^(-R/L)t

i(t) | (t=0) = 0.5A ===> C = 0.5

i(t) = 0.5 * exp^(-R/L)t
V(t) = L di / dt
     = L * 0.5 * (-R/L) * exp^(-R/L)t
	-(resistance / inductance)
#endif
