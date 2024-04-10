#pragma once

class runge_kutta
{
	double step;
	double x0;
	double vx0;
	double k1, k2, k3, k4;
	double k, k_suh, k_vyaz, l0;
	double m, g;
	double last_x, last_vx;
public:

	runge_kutta();

	runge_kutta(double h, double my_x0, double my_vx0, double my_k, double my_m, double my_k_s, double my_k_v, double my_l0);

	double func(double t, double x, double vx);

	double Fupr(double x);

	double sinX(double x);

	double cosX(double x);

	double FsTr(double x);

	double FvTr(double vx);

	void algRK(double t, double& x, double& vx);

	double AmplX();
};