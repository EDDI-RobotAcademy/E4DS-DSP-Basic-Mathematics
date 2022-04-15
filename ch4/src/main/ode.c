#include <stdio.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_odeiv2.h>

// y' = 2y + x
// https://www.wolframalpha.com/input/?i=y%27+%3D+2y+%2B+x%2C+y%280%29+%3D+1%2C+y%281%29
// https://www.wolframalpha.com/input/?i=1%2F4+*+%285+*+e%5E2+-+3%29
// 미분 방정식을 푸는 방법
// 1. 미분 방정식의 표준형을 작성한다.
//    y' <<<--- 미분항의 계수를 1로 만들어야 표준형이됨
// 2. 표준형의 y항 계수식을 통해 적분 인자를 구한다.
// 3. 적분 인자를 양변에 곱한다.
//    2y' + y = 3 -> y' + y / 2 = 3 / 2
//    여기서 적분 인자는 1 / 2가 됨
//    좀 더 정확하게는 exp^integral(1/2) 형태가 됨
// 4. 양변에 대한 계산(적분)을 진행한다.
// 5. 양변을 exp^적분인자 로 나누어 y에 대해 정리한다.
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
		// 0 ~ 10 - x input
        double xi = x0 + i * (xf-x0) / xf;
		// 결국 이 코드를 다시 재해석하자면
		// x가 0일 경우, 1일 경우, ... , 10일 경우에 대해 각각을 해석한다고 보면 됩니다!
		// 앞서서 룬게쿠타 4-5차 방식으로 미분 방정식을 계산하도록
		// odefunc을 만들었고 이에 대한 설정값들로 xi를 넣고 이에 대한 y값을 계산한다.
        int status = gsl_odeiv2_driver_apply (d, &x, xi, y);

        if (status != GSL_SUCCESS)
        {
            printf ("error, return value=%d\n", status);
            break;
        }

        printf ("%.8e %.8e\n", x, y[0]);
		printf ("x = %lf, xi = %lf\n", x, xi);
    }

	// malloc 혹은 new 처럼 Heap에 메모리를 할당하기 때문에
	// free를 사용해서 메모리 해제를 해주셔야 메모리 누수를 방지할 수 있음
    gsl_odeiv2_driver_free (d);
    return 0;
}
