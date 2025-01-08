#include "Complex.h"
Complex<int> operator""i(size_t a) {
	return { 0, int(a) };
}
Complex<double> operator""i(long double a) {
	return { 0, double(a) };
}