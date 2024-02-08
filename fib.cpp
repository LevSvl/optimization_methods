// МЕТОД ФИБОНАЧЧИ

#include<stdio.h>
#include<locale.h>
#include <cstdlib>

unsigned fib(unsigned n) {
	switch (n)
	{
	case 0: 
		return 1;
	case 1:
		return 1;
	default:
		return fib(n - 1) + fib(n - 2);
	}
}

float f(float x) {
	return 1/ (2 + x);
}

int main(){
	setlocale(LC_ALL,"RU");
	// Интервал неопределенности
	float a = 5.0, b = 100.0;
	// Задается количество вычислений 
	unsigned n;
	scanf("%d", &n);
	
	unsigned* fibArr = (unsigned*)malloc(sizeof(unsigned)*n);
	
	for (int i = n; i >= 0; i--) {
		fibArr[i] = fib(i);
	}

	float x1 = a + (float)(fibArr[n - 1] / fibArr[n]) * (b - a);
	float x2 = b - (float)(fibArr[n - 1] / fibArr[n]) * (b - a);

	float xOpt = x1;
	while (n>2) {


		float f1 = f(x1);
		float f2 = f(x2);

		if (f1 > f2) {
			xOpt = x2;
			b = x1;
			x2 = b - (float)(fibArr[n - 2] / fibArr[n - 1]) * (b - a);
		}
		else if (f1 < f2) {
			xOpt = x1;
			a = x2;
			x1 = a + (float)(fibArr[n - 2] / fibArr[n - 1]) * (b - a);
		}
		else
			xOpt = x1;

		n = n - 1;
	}
	printf("%f", f(xOpt));

	return 0;
}