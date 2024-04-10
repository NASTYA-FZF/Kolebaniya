#include "pch.h"
#include "Work.h"
#include <math.h>

runge_kutta::runge_kutta()
{
}

runge_kutta::runge_kutta(double h, double my_x0, double my_vx0, double my_k, double my_m, double my_k_s, double my_k_v, double my_l0)
{
	step = h;
	last_x = x0 = my_x0;
	last_vx = vx0 = my_vx0;
	k = my_k;
	m = my_m;
	k_suh = my_k_s;
	k_vyaz = my_k_v;
	l0 = my_l0;
	g = 10.;
}

double runge_kutta::func(double t, double x, double vx)
{
	//double direction = (vx > 0  )? 1.:-1;
	double for_minus;
	if (vx >= 0) for_minus = 1.;
	else for_minus = -1.;
	return (- for_minus * FsTr(x) - for_minus * FvTr(vx) - Fupr(x) * sinX(x)) / m;
}

double runge_kutta::Fupr(double x)
{
	return k * (sqrt(x * x + l0 * l0) - l0);
}

double runge_kutta::sinX(double x)
{
	return x / sqrt(x * x + l0 * l0);
}

double runge_kutta::cosX(double x)
{
	return l0 / sqrt(x * x + l0 * l0);
}

double runge_kutta::FsTr(double x)
{
	return k_suh * fabs(m * g - Fupr(x) * cosX(x));
}

double runge_kutta::FvTr(double vx)
{
	return k_vyaz * fabs(vx);
}

void runge_kutta::algRK(double t, double& x, double& vx)
{
	k1 = step * func(t, last_x, last_vx);
	k2 = step * func(t + step / 2, last_x + last_vx * step / 2, last_vx + k1 / 2);
	k3 = step * func(t + step / 2, last_x + last_vx * step / 2 + k1 * step / 4, last_vx + k2 / 2);
	k4 = step * func(t + step, last_x + last_vx * step + k2 * step / 2, last_vx + k3);

	x = last_x + last_vx * step + (k1 + k2 + k3) * step / 6;
	vx = last_vx + (k1 + 2 * k2 + 2 * k3 + k4) / 6;

	last_x = x;
	last_vx = vx;
}

double runge_kutta::AmplX()
{
	return sqrt((k * l0 * l0 / (k * l0 - m * g)) * (k * l0 * l0 / (k * l0 - m * g)) - l0 * l0);
}
