#include "FractalPattern.h"

FractalPattern::FractalPattern(const Complex<double>& limit, const size_t& steps) : limit(limit), steps(steps)
{	}

FractalPattern::~FractalPattern()
{
}

size_t FractalPattern::getSteps() const
{
	return steps;
}

Complex<double> FractalPattern::getLimit() const
{
	return limit;
}

JuliaSet::JuliaSet(const Complex<double>& limit, const size_t& steps, const Complex<double>& a, const Complex<double>& b, const size_t& power) : 
	FractalPattern(limit, steps), a(a), b(b), power(power)
{	}

std::pair<Complex<double>, size_t> JuliaSet::ComputePoint(const Complex<double>&point)
{
	double lim = abs(limit);
	Complex<double> ret = point;
	size_t i = 0;
	for (; i < steps && abs(ret) < lim; ++i) {
		ret = a * (pow(ret, power) + b);
	}
	return {ret, i};
}

JuliaSet::~JuliaSet()
{	}

MandelbrotSet::MandelbrotSet(const Complex<double>& limit, const size_t& steps, const size_t& power) :
	FractalPattern(limit, steps), power(power)
{
}

std::pair<Complex<double>, size_t> MandelbrotSet::ComputePoint(const Complex<double>& point)
{
	double lim = abs(limit);
	Complex<double> ret = point;
	size_t i = 0;
	for (; i < steps && abs(ret) < lim; ++i) {
		ret = pow(ret, power) + point;
	}
	return { ret, i };
}

MandelbrotSet::~MandelbrotSet()
{	}
