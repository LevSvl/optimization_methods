// лернд гнкнрнцн яевемхъ

#include <iostream>

double f(double x) {
	return 2 * x;
}

int main() {
	const double r = 0.618;
	
	double a = 5, b = 100, e = 0.0001;

	double xOpt = a + (b - a) * r;

	while (b - a > e) {
		double x1 = a + (b - a) * r;
		double x2 = b - (b - a) * r;

		double f1 = f(x1), f2 = f(x2);

		if (f1 > f2) {
			b = x1;
			xOpt = x2;
		}
		else if (f1 < f2) {
			a = x2;
			xOpt = x1;
		}
		else
			break;
	}

	std::cout << "Fopt(" << xOpt << ") = " << f(xOpt) << std::endl;


	return 0;
}