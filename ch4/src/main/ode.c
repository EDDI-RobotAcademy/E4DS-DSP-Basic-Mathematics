#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

// y' = 2y + x
// https://www.wolframalpha.com/input/?i=y%27+%3D+2y+%2B+x%2C+y%280%29+%3D+1%2C+y%281%29
// https://www.wolframalpha.com/input/?i=1%2F4+*+%285+*+e%5E2+-+3%29
int odefunc (double x, const double y[], double f[], void *params)
{
	// 계산하고자 하는 미분 방정식의 표준형식을 여기에 기록함
    f[0] = x+2*y[0];   
    return GSL_SUCCESS;
}

int * jac;

int main(void)
{
	// 1계 미분 방정식
	// 만약 2계 미분 방정식이라면 dim = 2
    int dim = 1;
	// 미방을 풀기 위한 데이터 타입
    gsl_odeiv2_system sys = {odefunc, NULL, dim, NULL};

	// 드라이버는 미방을 쉽게 풀 수 있도록 시간에 따른 변화, 제어, 스텝에 대한 래퍼
	// gsl_odeiv2_step_rkf45는 룬게쿠타 4-5차를 의미함
	// Taylor(테일러) 혹은 Maculaurin(맥클로린) 급수
	// 테일러 급수를 활용해서 미분을 4번이상 하여
	// 오차를 최소화하는 방식을 룬게쿠타 4-5차라고 표현함

	// 다음 인자는 미분방정식의 시작지점
	// 절대적인 에러 바운드와 상대적인 에러 바운드
    gsl_odeiv2_driver * d = gsl_odeiv2_driver_alloc_y_new (&sys, gsl_odeiv2_step_rkf45, 1e-6, 1e-6, 0.0);
    int i;
    double x0 = 0.0,  xf = 10.0; /* start and end of integration interval */
    double x = x0;
    double y[1] = { 1.0  };  /* initial value */

	// 루프를 돌면서 각 케이스별 미분 방정식을 계산함
    for (i = 1; i <= xf; i++)
    {
        double xi = x0 + i * (xf-x0) / xf;
		// 결국 이 코드를 다시 재해석하자면
		// x가 0일 경우, 1일 경우, ... , 10일 경우에 대해 각각을 해석한다고 보면 됩니다!
        int status = gsl_odeiv2_driver_apply (d, &x, xi, y);

        if (status != GSL_SUCCESS)
        {
            printf ("error, return value=%d\n", status);
            break;
        }

        printf ("%.8e %.8e\n", x, y[0]);
    }

	// malloc 혹은 new 처럼 Heap에 메모리를 할당하기 때문에
	// free를 사용해서 메모리 해제를 해주셔야 메모리 누수를 방지할 수 있음
    gsl_odeiv2_driver_free (d);
    return 0;
}
