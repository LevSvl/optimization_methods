// ������������ �������� �����������
// �������� - ���������
// ����������� - ������������ ���������
// ����������� - �������

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
/* ����������� ������� */
const double a = 5, b = 10;

double 
f(double x) // f(x) = 1 / (2 + x) - ���������
{
	return -(1 / (2 + x));
	//return sin(x);
}

/* ������������ �������� */

// ��������� ��������
#define NUM_OF_ITERS 25 // ������������ ��������
#define POPULATION_SIZE 1000 // ������ ���������
// ��������� ��������
#define T_SIZE 10 // ������ �������  2 <= T <= N
#define NUM_OF_MODELS_TO_CROSS_OVER POPULATION_SIZE // ���������� ������, ������������� � ����������
// ��������� �����������
#define p 0.6 // ����������� ����������� � ����
#define L sizeof(double) * 8 // ����������� ���������
// ��������� �������
# define pm 0.01 // ����������� �������



// ��������� �����
struct Model {
	double x;
	double fit;
};
// ��� "���������"
typedef vector<Model> Population;


/*
* // ��������������� �������
*/
double 
rand_double(int min, int max)
{
	// ��������� ������������ �����
	return (double) (min + rand() % (max - min + 1));
}

int 
rand_int(int min, int max)
{
	// ��������� ����� �����
	return (min + rand() % (max - min + 1));
}
template<typename T>
T 
bin_to_gray(T binary)
{
	T gray = binary ^ (binary >> 1);
	return gray;
}

template<typename T>
T
gray_to_bin(T gray) 
{
	T bin = 0;
	for (; gray; gray >> 1) {
		bin ^= gray;
	}
	return bin;
}

union Converter { uint64_t i; double d; };

double convert(std::bitset<64> const& bs) {
	Converter c;
	c.i = bs.to_ullong();
	return c.d;
}

double bitSwap(double a, double b, int position) {
	/*
	���������� c,� �������� ���� �� splitPosition �� a, ����� splitidx �� b
	*/
	std::bitset<sizeof(double) * 8> ba(*reinterpret_cast<unsigned long long*>(&a));
	std::bitset<sizeof(double) * 8> bb(*reinterpret_cast<unsigned long long*>(&b));
	std::bitset<sizeof(double) * 8> bresult;
	for (int i = 0; i < sizeof(double) * 8; i++) {
		if (i >= position){
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



// ������� ������������� ���������


void fitting
(Population &arr) 
{
	for (int iter_num = 0; iter_num < arr.size(); iter_num++) {
		double x = arr[iter_num].x;
		double fx;
		fx = (x >= a && x <= b) ? f(x) : 0;
		arr[iter_num].fit = fx;
	}
}

void 
selection(Population& arr)
{
	// ������� ������ ������ ���������� � �����������
	Population arr_to_cross_over;

	while (arr_to_cross_over.size() < NUM_OF_MODELS_TO_CROSS_OVER) {
		// ��������� �������� ���������� �������
		set<int> participants_idx;
		// �������� ���������� ������� ��������� �������
		while (participants_idx.size() < T_SIZE) {
			int random_idx = rand_int(0, arr.size());
			participants_idx.insert(random_idx);
		}
		// ����� ������ ���������� ������������ ������
		// ����� ����� � ������������ ��������� ��
		int max_fitness_model_idx = 0;
		for (auto m_idx : participants_idx) {
			if (arr[m_idx].fit > arr[max_fitness_model_idx].fit) {
				max_fitness_model_idx = m_idx;
			}
		}
		// ��������� ����� � ����� ��������� �����
		arr_to_cross_over.push_back(arr[max_fitness_model_idx]);
	}
	// �������� ������� ���������, ����������� �������
	arr = arr_to_cross_over;
}

void
cross_over(Population& arr) 
{
	// ������� ������ ���������
	// (��� ���� �� ����� ������� ����� ���������)
		// �������� �������� ��� ������� � ���������� � ������������
		// ���������� ������������ ����������� � ���� ����
		// ��������� ����� � ����������� � ������������ � �������� ���

	Population new_pop;
	while (new_pop.size() <= POPULATION_SIZE){
		int p1_idx = rand_int(0, POPULATION_SIZE - 1);
		int p2_idx = rand_int(0, POPULATION_SIZE - 1);
		if ((rand_int(1, 10) / 10) <= p) {
			//double p1_gen = arr[p1_idx].x;
			double p1_gen = arr[p1_gen].x;
			double p2_gen = arr[p2_gen].x;
			
			double child_gen = bitSwap(p1_gen,p2_gen, rand_int(2,L-1));

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
			arr[i].x = invertBit(arr[i].x,rand_int(2,L-1));
		}
	}
}



int 
main() 
{
	// �������� ������ ���������
	srand(time(NULL));

	Population population(POPULATION_SIZE);

	for (int iter_num = 0; iter_num < population.size(); iter_num++) {
		Model cur;
		cur.x = rand_double((int)a,(int)b);
		population[iter_num] = cur;
	}

	for (int iter_num = 0; iter_num < NUM_OF_ITERS; iter_num++) {
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