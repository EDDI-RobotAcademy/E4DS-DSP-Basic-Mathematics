#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

#include <GL/glut.h>

#define SLICE	360

int rhs (double t, const double y[], double f[], void *params_ptr);
//int jacobian (double t, const double y[], double *dfdy, double dfdt[], void *params_ptr);

void draw_y_x(void);

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

	draw_y_x();
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

void draw_y_x(void)
{
	float t = -100.0, step = 0.01;
	float x = 0, x2 = 0, y2, cx, cy;
	float tmp;
	int cache = 0;

	if(t > 100)
		t = 0.0;

	glBegin(GL_LINES);
	for(; ; t += step)
	{
		if(t > 100)
			break;

		y2 = t;

		if(cache)
		{
			// y'', y', y
			// z^-2, z^-1, z
			glVertex2f(cx, cy);	// 이전값
			glVertex2f(t, y2);	// 현재값

			// 이전 좌표 ~ 현재 좌표로 선을 긋는다.
			// GSL - GNU Scientific Library() - 미분방정식 라이브러리
			// 1계, 2계, RL Circuit Simulator
		}

		cache = 1;
		cx = t;
		cy = y2;
		//printf("t = %f, y2 = %f\n", t, y2);
	}
	glEnd();
}

int *jac;

int main (int argc, char **argv)
{
    int dimension = 2;		/* number of differential equations */
  
    double eps_abs = 1.e-8;	/* absolute error requested */
    double eps_rel = 1.e-10;	/* relative error requested */

    /* define the type of routine for making steps: */
    const gsl_odeiv_step_type *type_ptr = gsl_odeiv_step_rkf45;
    /* some other possibilities (see GSL manual):          
       = gsl_odeiv_step_rk4;
       = gsl_odeiv_step_rkck;
       = gsl_odeiv_step_rk8pd;
       = gsl_odeiv_step_rk4imp;
       = gsl_odeiv_step_bsimp;  
       = gsl_odeiv_step_gear1;
       = gsl_odeiv_step_gear2;
    */

    /* 
       allocate/initialize the stepper, the control function, and the
       evolution function.
    */
    gsl_odeiv_step *step_ptr = gsl_odeiv_step_alloc (type_ptr, dimension);
    gsl_odeiv_control *control_ptr = gsl_odeiv_control_y_new (eps_abs, eps_rel);
    gsl_odeiv_evolve *evolve_ptr = gsl_odeiv_evolve_alloc (dimension);

    gsl_odeiv_system my_system;	/* structure with the rhs function, etc. */

    double mu = 10;		/* parameter for the diffeq */
    double y[2];			/* current solution vector */

    double t, t_next;		/* current and next independent variable */
    double tmin, tmax, delta_t;	/* range of t and step size for output */

    double h = 1e-6;		/* starting step size for ode solver */

    /* load values into the my_system structure */
    my_system.function = rhs;	/* the right-hand-side functions dy[i]/dt */
    //my_system.jacobian = jacobian;	/* the Jacobian df[i]/dy[j] */
    my_system.jacobian = NULL;
    my_system.dimension = dimension;	/* number of diffeq's */
    //my_system.params = &mu;	/* parameters to pass to rhs and jacobian */
    my_system.params = NULL;

    tmin = 0.;			/* starting t value */
    tmax = 10.;			/* final t value */
    delta_t = 0.01;

    y[0] = 0.16;			/* initial x value */
    y[1] = 0.;			/* initial v value */

    t = tmin;             /* initialize t */
    printf ("%.5e %.5e %.5e\n", t, y[0], y[1]);	/* initial values */

    /* step to tmax from tmin */
    for (t_next = tmin + delta_t; t_next <= tmax; t_next += delta_t)
    {
        while (t < t_next)	/* evolve from t to t_next */
        {
            gsl_odeiv_evolve_apply (evolve_ptr, control_ptr, step_ptr,
                                    &my_system, &t, t_next, &h, y);
        }
        printf ("%.5e %.5e %.5e\n", t, y[0], y[1]); /* print at t=t_next */
    }

    gsl_odeiv_evolve_free (evolve_ptr);
    gsl_odeiv_control_free (control_ptr);
    gsl_odeiv_step_free (step_ptr);

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
	/*
    f[0] = y[1];
    f[1] = -y[0] + mu * y[1] * (1. - y[0] * y[0]);
	*/
	f[0] = y[1];
	f[1] = -y[1] - 9 * y[0];

    return GSL_SUCCESS;		/* GSL_SUCCESS defined in gsl/errno.h as 0 */
}

/*************************** Jacobian ****************************/
/*
*/

#if 0
int
jacobian (double t, const double y[], double *dfdy,
          double dfdt[], void *params_ptr)
{
    /* get parameter(s) from params_ptr; here, just a double */
    double mu = *(double *) params_ptr;

    gsl_matrix_view dfdy_mat = gsl_matrix_view_array (dfdy, 2, 2);

    gsl_matrix *m_ptr = &dfdy_mat.matrix;	/* m_ptr points to the matrix */

    /* fill the Jacobian matrix as shown */
    gsl_matrix_set (m_ptr, 0, 0, 0.0);	/* df[0]/dy[0] = 0 */
    gsl_matrix_set (m_ptr, 0, 1, 1.0);	/* df[0]/dy[1] = 1 */
    gsl_matrix_set (m_ptr, 1, 0, -2.0 * mu * y[0] * y[1] - 1.0); /* df[1]/dy[0] */
    gsl_matrix_set (m_ptr, 1, 1, -mu * (y[0] * y[0] - 1.0));     /* df[1]/dy[1] */

    /* set explicit t dependence of f[i] */
    dfdt[0] = 0.0;
    dfdt[1] = 0.0;

    return GSL_SUCCESS;		/* GSL_SUCCESS defined in gsl/errno.h as 0 */
}
#endif
