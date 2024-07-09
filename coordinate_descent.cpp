#include<iostream>
#include<iomanip>
#include<cmath>
#include<vector>

using namespace std;

double f(vector<double> vec) {
	double x1 = vec[0];
	double x2 = vec[1];
	return pow((x1 * x1 + x2 - 11), 2) + pow((x1 + x2 * x2 - 7), 2);
}

int main() {
	vector<double> xk = {1,1};

	double e = 0.0001; // точность
	double lambda = 0.001; // шаг
	double delta = 2.0 * e;

	int cnt = 0;
	while (delta > e) {
		// копирование предыдущей итерации для сравнения 
		vector<double> xkPrev = xk;
		// Одномерная минимизация для каждой функции
		for (int i = 0; i < xk.size(); i++) {

			double fxk = f(xk);
			// xLeft
			double xLeft = xk[i] - lambda;
			vector<double> newXk = xk;
			newXk[i] = xLeft;
			// f(xLeft...)
			double fxLeft = f(newXk);
			

			if (fxLeft < fxk) { // Движение влево
				double fxLeft1 = fxLeft;
				do {
					xk[i] = xLeft;
					
					xLeft -= lambda;
					vector<double> newXkLeft = xk;
					newXkLeft[i] = xLeft;

					fxLeft1 = f(newXkLeft);

				} while (fxLeft1 < f(xk));

			}
			else { // Движение вправо
				double xRight = xk[i] + lambda;

				vector<double> newXkRight = xk;
				xk[i] = xRight;

				if (f(newXkRight) < f(xk)) {

					double fxRight1 = f(newXkRight);
					do {
						xk[i] = xRight;

						xRight += lambda;
						vector<double> newXkRight = xk;
						newXkRight[i] = xRight;

						fxRight1 = f(newXkRight);

					} while (fxRight1 < f(xk));
				}
			}
			
		}

		delta = abs(f(xk) - f(xkPrev));



		//delta = abs(xk[xMaxIdx] - xkPrev[xMaxIdx]);
		
		//cout << "F(x1,x2) -> min = " << f(xk) << ", x1 = " << xk[0] << ", x2 = " << xk[1] << endl;
		//cout << "val    "<<delta << "         _____iter______          " << ++cnt << endl;
	}
	
	cout << "F(x1,x2) -> min = " << std::fixed << f(xk) << ", x1 = " << xk[0] << ", x2 = " << xk[1] << endl;

	return 0;
}