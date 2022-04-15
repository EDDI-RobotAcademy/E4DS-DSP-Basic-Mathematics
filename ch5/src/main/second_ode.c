/* 
   The following program solves the second-order nonlinear 
   Van der Pol oscillator equation (see Landau/Paez 14.12, part 1),
   x"(t) + \mu x'(t) (x(t)^2 - 1) + x(t) = 0
   This can be converted into a first order system suitable for 
   use with the library by introducing a separate variable for 
   the velocity, v = x'(t).  We assign x --> y[0] and v --> y[1].
   So the equations are:
   x' = v                  ==>  dy[0]/dt = f[0] = y[1]
   v' = -x + \mu v (1-x^2) ==>  dy[1]/dt = f[1] = -y[0] + mu*y[1]*(1-y[0]*y[0])
   x"(t) = - \mu x'(t) (x(t)^2 - 1) - x(t)

   x''(t) + x'(t) + x(t) = 0
   x''(t) = -x'(t) - x(t)

   x' = v ===> dy[0]/dt = f[0] = y[1]
   v' = -v - x
*/

#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv.h>

/* function prototypes */
int rhs (double t, const double y[], double f[], void *params_ptr);
//int jacobian (double t, const double y[], double *dfdy, double dfdt[], void *params_ptr);

/*************************** main program ****************************/

int *jac;

int
main (void)
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
	// 룬게쿠타 방식의 2계 미분 방정식으로 stepping을 진행합니다.
    gsl_odeiv_step *step_ptr = gsl_odeiv_step_alloc (type_ptr, dimension);
	// 허용 오차
    gsl_odeiv_control *control_ptr = gsl_odeiv_control_y_new (eps_abs, eps_rel);
	// 계산하는 차원
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
    delta_t = 1.;

	// 주의점이라면 2계 미분 방정식은 최소 2개의 초기값이 필요하다는 부분을 주의!
    y[0] = 1.;			/* initial x value */
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
		// y[0], y[1] 결과 확인이 필요함
        printf ("%.5e %.5e %.5e\n", t, y[0], y[1]); /* print at t=t_next */
    }

    /* all done; free up the gsl_odeiv stuff */
    gsl_odeiv_evolve_free (evolve_ptr);
    gsl_odeiv_control_free (control_ptr);
    gsl_odeiv_step_free (step_ptr);

    return 0;
}

/*************************** rhs ****************************/
/* 
   x''(t) + x'(t) + x(t) = 0
   x''(t) = -x'(t) - x(t)

   x' = v            ===> dy[0]/dt = f[0] = y[1]
   v' = -v - x       ===> dy[1]/dt = f[1] = -y[1] - y[0]
*/

// https://www.wolframalpha.com/input/?i=y%27%27+%2B+y%27+%2B+y+%3D+0%2C+y%280%29+%3D+1%2C+y%27%280%29+%3D+0, y(0) = 1
// https://www.wolframalpha.com/input/?i=%28sqrt%283%29+*+sin%28sqrt%283%29+%2F+2%29+%2B+3+*+cos%28sqrt%283%29+%2F+2%29%29+%2F+%283+*+sqrt%28e%29%29, y(1)
// (sqrt(3) * sin(sqrt(3) / 2) + 3 * cos(sqrt(3) / 2)) / (3 * sqrt(e)), y(1) = 0.6597001...
int
rhs (double t, const double v[], double f[], void *params_ptr)
//rhs (double t, const double y[], double f[], void *params_ptr)
{
	/*
    f[0] = y[1];
    f[1] = -y[0] + mu * y[1] * (1. - y[0] * y[0]);
	*/
	// * 코드 작성 팁: 미분항은 f에 배치합니다.
	// 1계와 마찬가지로 반드시 표준형이어야 합니다!
	// 첫번째 1차 미분이 무엇(치환)입니다 ~
	f[0] = v[1];
	// 두번째 2차 미분을 치환을 기반으로 어떻게 표현해야 하는지를 기록합니다.
	f[1] = -v[1] - v[0];
	/* Tip: 배열의 인덱스는 결국 미분의 횟수라고 생각해도 좋습니다 */
	// 현재 초기값이 있으니까 두 개의 근이 나오지 않고 한 개의 근이 나옴!

    return GSL_SUCCESS;		/* GSL_SUCCESS defined in gsl/errno.h as 0 */
}

/*************************** Jacobian ****************************/
/*
  Define the Jacobian matrix using GSL matrix routines.
  (see the GSL manual under "Ordinary Differential Equations") 
  
  * params is a void pointer that is used in many GSL routines
  to pass parameters to a function
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
