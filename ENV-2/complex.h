#include <iostream>
#include <math.h>

#ifndef SORTINGALGORITHM_H
#define SORTINGALGORITHM_H

 
class Complex{

private:
  double  real_part;
  double im_part;

public:

Complex(double real = 0.0, double  im = 0.0);

Complex operator+(const Complex& other) const;

Complex operator-(const Complex& other) const;

Complex operator*(double const_) const;

double absolute_value() const;

friend std::ostream& operator<<(std::ostream& os, const Complex& comp);
    
};

#endif
