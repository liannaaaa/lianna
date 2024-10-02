#include "sort.h"

void sortComplex(Complex arr[], size_t size) {

    for (size_t i = 1; i < size; i++) {
        Complex key = arr[i];
        size_t j = i - 1;

     while (j >= 0 && arr[j].absolute_value() > key.absolute_value()) {
	   arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

