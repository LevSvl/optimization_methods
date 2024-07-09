#include <iostream>
#include <bitset>

union Converter { uint64_t i; double d; };

double convert(std::bitset<64> const& bs) {
    Converter c;
    c.i = bs.to_ullong();
    return c.d;
}

int main() {
    double number = -1.5; // Ваше значение double
    std::bitset<sizeof(double) * 8> bits(*reinterpret_cast<unsigned long long*>(&number));

    std::cout << "All bits of the double number: " << bits << std::endl;

    std::cout << "All bits of the double number: " << convert(bits) << std::endl;


    std::bitset<64> y;
    for (int i = 0; i < 64; i++) {
        y[i] = !bits[i];
    }
    std::cout << "All bits of the double number: " << convert(y) << std::endl;
    return 0;
}