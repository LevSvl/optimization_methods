// дхунрнлхвеяйхи лернд

#include<stdio.h>
#include<cstdlib>

float f(float x) {
	return 1 / (2+x);
}

int main() {
	float a = -1;
	float b = 4;
	float e = 0.001;

	float mid;
	
	while (b - a > 2.0 * e) {
		mid = (b + a) / 2;

		float x1 = mid - e / 2;
		float x2 = mid + e / 2;

		if (f(x1) > f(x2))
			a = x1;
		else if (f(x1) < f(x2))
			b = x2;
		else
			break;
	}
	printf("optimum = %f", mid);

	return 0;
}