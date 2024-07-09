// Генетический алгоритм минимизации
// Селекция - рулеточная
// Скрещивание - одноточечный кросоввер
// Кодирование - обычное

#include <iostream>
#include <bitset>
#include <cmath>
#include <vector>
#include <set>
#include <ctime>
using namespace std;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
/* Исследуемая функция */
const double a = 5, b = 10;

double
f(double x) // f(x) = 1 / (2 + x) - гипербола
{
	return -(1 / (2 + x));
	//return sin(x);
}

/* Генетический алгоритм */

// Параметры эволюции
#define NUM_OF_ITERS 25 // Длительность эволюции
#define POPULATION_SIZE 100 // Размер популяции, должно быть кратно 2
// Параметры селекции
#define NUM_OF_MODELS_TO_CROSS_OVER POPULATION_SIZE/2 // Количество особей, допускающихся к срещиванию
// Параметры скрещивания
#define p 0.6 // Вероятность объединения в пару
#define L sizeof(double) * 8 // Размерность хромосомы
// Параметры мутации
# define pm 0.01 // Вероятность мутации

// Структура особи
struct Model {
	double x;
	double fit;
	static double all_fit;
};
// Тип "Популяция"
typedef vector<Model> Population;


// Вспомогательные функции
double
rand_double(int min, int max)
{
	// Случайное вещественное число
	return (double)(min + rand() % (max - min + 1));
}

int
rand_int(int min, int max)
{
	// Случайное целое число
	return (min + rand() % (max - min + 1));
}


union Converter { uint64_t i; double d; };

double convert(std::bitset<64> const& bs) {
	Converter c;
	c.i = bs.to_ullong();
	return c.d;
}

double bitSwap(double a, double b, int position) {
	/*
	Возвращает c,у которого биты до splitPosition от a, после splitidx от b
	*/
	std::bitset<sizeof(double) * 8> ba(*reinterpret_cast<unsigned long long*>(&a));
	std::bitset<sizeof(double) * 8> bb(*reinterpret_cast<unsigned long long*>(&b));
	std::bitset<sizeof(double) * 8> bresult;
	for (int i = 0; i < sizeof(double) * 8; i++) {
		if (i >= position) {
			bresult[i] = ba[i];
		}
		else {
			bresult[i] = bb[i];
		}
	}
	double result = convert(bresult);
	return result;
}

double invertBit(double a, int position) {
	std::bitset<sizeof(double) * 8> ba(*reinterpret_cast<unsigned long long*>(&a));
	ba[position] = !ba[position];
	return convert(ba);
}


// Функции генетического алгоритма

void fitting
(Population& arr)
{
	for (int iter_num = 0; iter_num < arr.size(); iter_num++) {
		double x = arr[iter_num].x;
		double fx;
		fx = (x >= a && x <= b) ? f(x) : 0;
		arr[iter_num].fit = fx; 
		arr[iter_num].all_fit += fx;
	}
}
void
selection(Population& arr)
{

	// Создаем массив особей отобранных к скрещиванию
	Population arr_to_cross_over;
	set<int> indexes_of_added;

	while (arr_to_cross_over.size() < NUM_OF_MODELS_TO_CROSS_OVER) {
		int random_model_idx = rand_int(0, POPULATION_SIZE - 1);
		
		// Рассчитать размер сектора и добавить с соответствующей вероятностью
		double fx = arr[random_model_idx].fit; 
		double x = arr[random_model_idx].x;
		double fitting_sum = arr[random_model_idx].all_fit;
		double sector_size = (fitting_sum != 0) ? fx/fitting_sum : 1 / arr.size();
		if (fx == 0 || sector_size == 0) {
			continue;
		}

		// При большом количестве особей размер сектора отдельной особи
		// становится крайне мал. Чтобы получить соответствующую вероятность,
		// необходимо получить такой же точности случайное значение
		double chance_to_be_selected;
		chance_to_be_selected = ((double)rand() / (double)(RAND_MAX / 1));
		if (chance_to_be_selected<= sector_size){
			arr_to_cross_over.push_back(arr[random_model_idx]);
		}
	}
	// Заменяем текущую популяцию, отобранными особями
	arr = arr_to_cross_over;
}

void
cross_over(Population& arr)
{
	// Создать пустую популяцию
	// (или пока не будет набрана новая популяцмя)
		// рандомно выбирать два индекса и скрещивать с вероятностью
		// скрещивать одноточечным кроссовером в коде грея
		// добавлять особь с результатом в переведенном в бинарный вид

	Population new_pop;
	while (new_pop.size() <= POPULATION_SIZE) {
		int p1_idx = rand_int(0, POPULATION_SIZE - 1);
		int p2_idx = rand_int(0, POPULATION_SIZE - 1);
		if ((rand_int(1, 10) / 10) <= p) {
			//double p1_gen = arr[p1_idx].x;
			double p1_gen = arr[p1_gen].x;
			double p2_gen = arr[p2_gen].x;

			double child_gen = bitSwap(p1_gen, p2_gen, rand_int(2, L - 1));

			Model ch; ch.x = child_gen;
			new_pop.push_back(ch);
		}
	}
	arr = new_pop;
}

void
mutation(Population& arr)
{
	int mutate_or_not = 0;
	for (int i = 0; i < arr.size(); i++) {
		mutate_or_not = rand_int(1, 100);
		if (mutate_or_not <= (int)(pm * 100)) {
			arr[i].x = invertBit(arr[i].x, rand_int(2, L - 1));
		}
	}
}


double Model::all_fit = 0;
int
main()
{
	// Создание первой популяции
	srand(time(NULL));

	Model::all_fit = 0;

	Population population(POPULATION_SIZE);
	for (int iter_num = 0; iter_num < population.size(); iter_num++) {
		Model cur;
		cur.x = rand_double((int)a, (int)b);
		population[iter_num] = cur;
	}

	for (int iter_num = 0; iter_num < NUM_OF_ITERS; iter_num++) {
		// на каждой итерации сбрасываем общую сумму значений ЦФ
		//Model dummy;
		Model::all_fit = 0;


		fitting(population);
		selection(population);
		cross_over(population);
		mutation(population);
	}
	int max_idx = 0;
	for (int i = 0; i < population.size(); i++) {
		if (population[i].fit > population[max_idx].fit) {
			max_idx = i;
		}
	}
	Model best = population[max_idx];
	std::cout << "model best x = " << best.x << std::endl;

	return 0;
}