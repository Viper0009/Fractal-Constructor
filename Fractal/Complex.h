#pragma once
#include <concepts>
#include <iostream>
template<typename T>
concept Number = std::floating_point<T> || std::integral<T>;
template<Number T = float>
class Complex {
private:
public:
	T real, imag;
	Complex() : real(), imag() {}
	Complex(const T& re, const T& im) : real(re), imag(im) {}
	Complex(const T& re) : real(re), imag(0) {}
	Complex<T> operator+(const Complex<T>& v) const {
		return { real + v.real, imag + v.imag };
	}
	Complex<T> operator-(const Complex<T>& v) const {
		return { real - v.real, imag - v.imag };
	}
	Complex<T> operator*(const Complex<T>& v) const {
		return { real * v.real - imag * v.imag, real * v.imag + v.real * imag };
	}
	template<Number T1>
	operator Complex<T1>() const {
		return { T1(real), T1(imag) };
	}
};
template<Number T>
static Complex<T> pow(const Complex<T>& v, size_t t) {
	if (t == 0) {
		return { 1, 0 };
	}
	else if (t % 2 == 1) {
		return pow(v * v, t / 2) * v;
	}
	else
		return pow(v * v, t / 2);
}
template<Number T>
static T abs(const Complex<T>& v) {
	return sqrt(v.real * v.real + v.imag * v.imag);
}
template<Number T, Number T1>
Complex<T> operator+(const T1& r, const Complex<T>& v) {
	return { T(r) + v.real, v.imag };
}
template<Number T, Number T1>
Complex<T> operator-(const T1& r, const Complex<T>& v) {
	return { T(r) - v.real, -v.imag };
}
template<Number T>
std::ostream& operator<<(std::ostream& os, const Complex<T>& v) {
	os << '(' << v.real << ';' << v.imag << ')';
	return os;
}
template<Number T>
std::istream& operator>>(std::istream& is, Complex<T>& v) {
	is.ignore(std::numeric_limits<std::streamsize>::max(), '(');
	is >> v.real;
	is.ignore(std::numeric_limits<std::streamsize>::max(), ',');
	is >> v.imag;
	is.ignore(std::numeric_limits<std::streamsize>::max(), ')');
	return is;
}
Complex<int> operator""i(size_t a);
Complex<double> operator""i(long double a);