#include <stdio.h>
#include <gsl/gsl_math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

// y' = exp(x)
// https://www.wolframalpha.com/input/?i=y%27+%3D+2y+%2B+x%2C+y%280%29+%3D+1%2C+y%281%29
// https://www.wolframalpha.com/input/?i=1%2F4+*+%285+*+e%5E2+-+3%29
int odefunc (double x, const double y[], double f[], void *params)
{
    // f[0] = x+2*y[0];
	// pow(): 멱급수(a, b) = a^b (a의 b승)
	// math.h에 있으며 pow를 활용하기 위해서는 -lm 옵션이 추가로 필요합니다.
	// GSL 라이브러리를 활용하기 위해선 -lgsl 옵션이 필요합니다.
	f[0] = pow(M_E, x);

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
    double y[1] = { 1 };  /* initial value */
	double tmp = 0.0;

    for (i = 0; tmp <= xf; tmp += 0.1)
    {
		// for문을 포함하여 아래 한 줄을 통해 상황에 따라
		// 간소화된 형태로 결과를 출력할 수도 있고
		// 시간이 좀 더 오래걸리더라도 더 정확한 데이터를 산출할 수도 있음
        double xi = x0 + tmp * (xf-x0) / xf;
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
