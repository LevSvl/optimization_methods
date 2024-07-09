#include<iostream>
#include<cmath>
#include<vector>
using namespace std;


//											Градентный спуск
//double f(double x1) {
//	return x1 * x1 + 5 * x1;
//}
//
//double df_dx(double x1) {
//	return 2*x1 + 5;
//}
//
//int gradient() {
//	double eps = 0.001;
//	double lambda = 0.1; // Начальный шаг
//
//	double xk = 1;
//
//	double delta = 2 * eps;
//
//	while (delta > eps) {
//		double xk_prev = xk;
//
//		double grad = df_dx(xk);
//		xk = xk - grad * lambda;
//
//		delta = abs(f(xk) - f(xk_prev));
//	}
//
//	cout << "f(" << xk << ") = " << f(xk) << endl;
//
//	return 0;
//}

//											Наискорейший спуск
double ff(double x1, double x2) {
	return x1*x1 + 4*x2*x2 - 4*x1 - 8*x2 + 5;
}

double f(double x[2]) {
	double x1 = x[0], x2 = x[1];
	return x1 * x1 + 4 * x2 * x2 - 4 * x1 - 8 * x2 + 5;
}

double df_dx1(double x1) {
	return 2*x1 - 4;
}

double df_dx2(double x2) {
	return 8 * x2 - 8;
}

double norm_grad(int i, double x1, double x2) {
	double dx1 = df_dx1(x1);
	double dx2 = df_dx2(x2);
	double norm = 1 / sqrt(dx1*dx1 + dx2*dx2);
	double grad;
	switch(i) {
	case 0:
		grad = dx1 * norm;
		break;
	case 1:
		grad = dx2 * norm;
		break;
	}
	return grad;
}

int main() {
	double eps = 0.001;
	double lambda = 0.1; // Начальный шаг

	double xk[2] = {1,1};

	double delta = 2 * eps;

	while (delta > eps) {
		double xk_prev[2];
		memcpy((void*)xk_prev, (void*)xk, sizeof(xk));


		for (int i = 0; i < 2; i++) {
			double grad = norm_grad(i, xk[0], xk[1]);

			double x_last_arr[2];
			memcpy((void*)x_last_arr, (void*)xk, sizeof(xk));
			double x_last = xk[i];

			double x_current_arr[2];
			memcpy((void*)x_current_arr, (void*)xk, sizeof(xk));
			x_current_arr[i] = xk[i] - lambda * grad;

			while (f(x_current_arr) < f(x_last_arr)) {
				x_last_arr[i] = x_current_arr[i];
				x_current_arr[i] = x_current_arr[i] - lambda * grad;
			}

			xk[i] = x_last_arr[i];
		}
		double fxk = f(xk);
		double fx_prev = f(xk_prev);
		//delta = abs(f(xk[0], xk[1]) - f(xk_prev[0], xk_prev[1]));
		delta = abs(fxk - fx_prev);
		lambda = (fxk > fx_prev) ? fx_prev : fxk;
	}
	//cout << "f(" << xk[0] << "," << xk[1] << ") = " << f(xk[0], xk[1]) << endl;
	cout << "f(" << xk[0] << "," << xk[1] << ") = " << f(xk) << endl;
	char c = getchar();

	return 0;
}