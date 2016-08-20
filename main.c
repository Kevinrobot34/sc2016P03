#include <stdio.h>
#include <stdlib.h>
#include <math.h>

//#define p_0 1.5
#define c_0 (299.79)

double B_data[500], r_data[500];
double p_0;
int max_data = 0;

double B(double r){
	//if (r < 0.2) return 0.015;
	//else return 0.001;

	int i;
	for (i = 0; i < max_data; i++) {
		if (r < r_data[i]) break;
	}
	if (i == 0) {
		return B_data[0];
	} else {
		return B_data[i] + (B_data[i] - B_data[i-1]) / (r_data[i] - r_data[i - 1]) * (r - r_data[i]);
	}
}

double calc_dx(double x, double y, double theta) {
	double r = sqrt(x * x + y * y);
	double rho = p_0 / (c_0 * B(r));

	return (rho * sin(theta));
}
double calc_dy(double x, double y, double theta) {
	double r = sqrt(x * x + y * y);
	double rho = p_0 / (c_0 * B(r));

	return (rho * cos(theta));
}

int main(int argc, char** argv) {
	double theta_0 = 0.0; // 初期角度Y軸からの角度
	//const double p = 1.0; // 運動量
	//const double c = 3.0 * pow(10, 8); // 光速
	const double dtheta = 0.01; // 角度の刻み幅
	const double theta_max = 3 * M_PI / 4;
	const double x_0 = 0.0;
	const double y_0 = - 0.4; // [m]
	double rho;
	double theta;
	double x, y, dx, dy;
	double dx1, dx2, dx3, dx4;
	double dy1, dy2, dy3, dy4;
	int i;

	if (argc < 3) {
		fprintf(stderr, "Usage: %s p[MeV] theta_0[rad]\n", argv[0]);
		exit(1);
	}
	p_0 = atof(argv[1]);
	theta_0 = atof(argv[2]);

	i = 0;
	while (scanf("%lf,%lf", &r_data[i], &B_data[i]) != EOF) {
		r_data[i] = r_data[i] / 1000.0; // [mm] -> [m]
		B_data[i] = B_data[i] / 1000.0; // [mT] -> [T]
		i++;
	}
	max_data = i;

	x = x_0;
	y = y_0;

	printf("#theta[rad] x[mm] y[mm]\n");
	printf("%lf %lf %lf\n", theta_0, x * 1000.0, y * 1000.0);
	for (theta = theta_0; theta < theta_max; theta += dtheta) {
		//r = sqrt(x * x + y * y);
		//rho = p / (c * B(r));
		
		dx1 = calc_dx(x, y, theta) * dtheta;
		dy1 = calc_dy(x, y, theta) * dtheta;

		dx2 = calc_dx(x + dx1 / 2.0, y + dy1 / 2.0, theta) * dtheta;
		dy2 = calc_dy(x + dx1 / 2.0, y + dy1 / 2.0, theta) * dtheta;

		dx3 = calc_dx(x + dx2 / 2.0, y + dy2 / 2.0, theta) * dtheta;
		dy3 = calc_dy(x + dx2 / 2.0, y + dy2 / 2.0, theta) * dtheta;

		dx4 = calc_dx(x + dx3, y + dy3, theta) * dtheta;
		dy4 = calc_dy(x + dx3, y + dy3, theta) * dtheta;

		x = x + (dx1 + 2.0 * dx2 + 2.0 * dx3 + dx4)/6.0;
		y = y + (dy1 + 2.0 * dy2 + 2.0 * dy3 + dy4)/6.0;
		printf("%lf %lf %lf\n", theta, x * 1000.0, y * 1000.0);
		//printf("  %lf %lf %lf %lf\n", dx1, dx2, dx3, dx4);
	}

	return 0;
}
