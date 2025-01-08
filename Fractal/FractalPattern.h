#pragma once
#include "Complex.h"
class FractalPattern {
protected:
	Complex<double> limit;
	size_t steps;
public:
	FractalPattern(const Complex<double>& limit, const size_t& steps);
	virtual std::pair<Complex<double>, size_t> ComputePoint(const Complex<double>& point) = 0;
	virtual ~FractalPattern();
	size_t getSteps() const;
	Complex<double> getLimit() const;
};
class JuliaSet : public FractalPattern {
protected:
	Complex<double> a, b;
	size_t power;
public:
	JuliaSet(const Complex<double>& limit, const size_t& steps, const Complex<double>& a, const Complex<double>& b, const size_t& power = 2);
	virtual std::pair<Complex<double>, size_t> ComputePoint(const Complex<double>& point) override;
	~JuliaSet();
};
class MandelbrotSet : public FractalPattern {
protected:
	Complex<double> additional;
	size_t power;
public:
	MandelbrotSet(const Complex<double>& limit, const size_t& steps, const size_t& power = 2);
	virtual std::pair<Complex<double>, size_t> ComputePoint(const Complex<double>& point) override;
	~MandelbrotSet();
};
