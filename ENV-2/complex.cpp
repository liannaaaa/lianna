#include "main.cpp"

Complex::Complex(double real = 0.0, double  im = 0.0):real_part(real), im_part(im){}

Complex Complex::operator+(const Complex& other) const{
    return Complex(real_part + other.real_part, im_part + other.im_part);
}

Complex Complex::operator-(const Complex& other) const{
    return Complex(real_part - other.real_part, im_part - other.im_part);
}

Complex Complex::operator*(double const_) const{
    return Complex(real_part * const_, im_part * const_);
}

double Complex::absolute_value() const{
 return std::sqrt(real_part * real_part + im_part * im_part);
}

friend Complex::std::ostream& operator<<(std::ostream& os, const Complex& comp) {
   if(comp.im_part >= 0){
    os << comp.real_part << "+" << comp.im_part << "i";
}
   if(comp.im_part < 0){
        os << comp.real_part << "" << comp.im_part<< "i";
   }
        return os;
 }


