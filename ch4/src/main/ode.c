#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

// y' = 2y + x
// https://www.wolframalpha.com/input/?i=y%27+%3D+2y+%2B+x%2C+y%280%29+%3D+1%2C+y%281%29
// https://www.wolframalpha.com/input/?i=1%2F4+*+%285+*+e%5E2+-+3%29
int odefunc (double x, const double y[], double f[], void *params)
{
    f[0] = x+2*y[0];   
    return GSL_SUCCESS;
}

int * jac;

int main(void)
{
    int dim = 1;
	// 미방을 풀기 위한 데이터 타입
    gsl_odeiv2_system sys = {odefunc, NULL, dim, NULL};

	// 드라이버는 미방을 쉽게 풀 수 있도록 시간에 따른 변화, 제어, 스텝에 대한 래퍼
	// gsl_odeiv2_step_rkf45는 룬게쿠타 4-5차를 의미함
	// 다음 인자는 미분방정식의 시작지점
	// 절대적인 에러 바운드와 상대적인 에러 바운드
    gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rkf45, 1e-6, 1e-6, 0.0);
    int i;
    double x0 = 0.0,  xf = 10.0; /* start and end of integration interval */
    double x = x0;
    double y[1] = { 1.0  };  /* initial value */

    for (i = 1; i <= xf; i++)
    {
        double xi = x0 + i * (xf-x0) / xf;
        int status = gsl_odeiv2_driver_apply (d, &x, xi, y);

        if (status != GSL_SUCCESS)
        {
            printf ("error, return value=%d\n", status);
            break;
        }

        printf ("%.8e %.8e\n", x, y[0]);
    }

    gsl_odeiv2_driver_free (d);
    return 0;
}
