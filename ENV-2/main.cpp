#include <iostream>
#include "complex.h"
#include "sort.h"

int main() {
    Complex ComplexNumbers[] = {
        Complex(3, 4),
        Complex(1, 1),
        Complex(2, -3),
    };
    size_t size = sizeof(ComplexNumbers) / sizeof(ComplexNumbers[0]);

    for (size_t i = 0; i < size; i++) {
        std::cout << ComplexNumbers[i] << std::endl;
    }

    sortComplex(ComplexNumbers, size);

    for (size_t i = 0; i < size; i++) {
        std::cout << ComplexNumbers[i] << std::endl;
    }

    return 0;
}
