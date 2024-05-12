// File: Polynomial.h
// Author: Ryan P.
// Timestamp: <30-Mar-2024> 22:21:34 PDT
// Purpose: Class which represents a polynomial

#include <iomanip>
#include <stdexcept>
#include <stdlib.h>
#include <sstream>

#include "Polynomial.h"

#define EPSILON std::numeric_limits<float>::epsilon()

Polynomial::Polynomial()
{
	coefficients.push_back(0);
}

Polynomial::Polynomial(float a)
{
	coefficients.push_back(a);
}

Polynomial::Polynomial(const std::vector<float>& values)
{
	for (int val : values)
	{
		coefficients.push_back(val);
	}

	if (values.empty())
		coefficients.push_back(0);
}

Polynomial::~Polynomial()
{

}

Polynomial Polynomial::operator+(const Polynomial& right) const
{
	std::vector<float> newCoefficients;

	int higherDegree = GetDegree();
	if (right.GetDegree() > higherDegree)
		higherDegree = right.GetDegree();

	// Add corresponding coefficients.
	for (int i = 0; i <= higherDegree; i++)
	{
		float leftCoefficent = 0;
		float rightCoefficient = 0;

		if (i <= GetDegree())
			leftCoefficent = coefficients[i];

		if (i <= right.GetDegree())
			rightCoefficient = right.coefficients[i];

		newCoefficients.push_back(leftCoefficent + rightCoefficient);
	}

	// Remove zeroes from the end of the coefficient list
	while (newCoefficients.size() > 1 && newCoefficients.back() == 0)
	{
		newCoefficients.pop_back();
	}

	return Polynomial(newCoefficients);
}

// Get the additive inverse of this polynomial. Aka, just multiply it by -1.
Polynomial Polynomial::operator-() const
{
	std::vector<float> newCoefficients;

	for (int i = 0; i < GetDegree(); i++)
	{
		newCoefficients.push_back(-coefficients[i]);
	}

	return Polynomial(newCoefficients);
}

Polynomial Polynomial::operator-(const Polynomial& right) const
{
	return this->operator+(-right);
}

Polynomial Polynomial::operator*(const Polynomial& right) const
{
	std::vector<float> newCoefficients;

	int maximumNewDegree = GetDegree() + right.GetDegree();

	// Formula for the nth coefficient of the product of two arbitrary polynomials:
	// c_n = a_0*b_n + a_1*b_(n-1) a_2*b_(n-2) + a_3*b_(n-3) + ... + a_(n-1)b_1 + a_n*b_0
	for (int i = 0; i < maximumNewDegree; i++)
	{
		int ithCoeff = 0;
		
		for (int j = i; j >= 0; j--)
		{
			float leftCoefficent = 0;
			float rightCoefficient = 0;

			if (i <= GetDegree())
				leftCoefficent = coefficients[i];

			if (i <= right.GetDegree())
				rightCoefficient = right.coefficients[j];
			
			ithCoeff += leftCoefficent * rightCoefficient;
		}

		newCoefficients.push_back(ithCoeff);
	}

	// Remove zeroes from the end of the coefficient list
	while (newCoefficients.size() > 1 && newCoefficients.back() == 0)
	{
		newCoefficients.pop_back();
	}

	return Polynomial(newCoefficients);
}

Polynomial Polynomial::operator/(const Polynomial& divisor) const
{
	if (divisor == Polynomial(0))
	{
		throw std::invalid_argument("Division by 0 is not defined.");
	}
	
	Polynomial quotient(0);
	Polynomial remainder(*this); // At each step, n = d * q + r

	while (remainder != Polynomial(0) && remainder.GetDegree() >= divisor.GetDegree())
	{
		Polynomial temp = Polynomial(remainder.coefficients[0] / divisor.coefficients[0]);
		quotient = quotient + temp;
		remainder = remainder - temp * divisor;

	}

	return quotient;
}

// This operator should give us the least residue modulo "mod"
// For polynomial rings over a field, this happens to be the remainder upon division by the modulus.
Polynomial Polynomial::operator%(const Polynomial& mod) const
{
	if (mod == Polynomial(0))
	{
		// Since f(x) - f(x) = 0, then
		// f(x) is congruent to f(x) mod 0. 
		return *this;
	}

	Polynomial quotient(0);
	Polynomial remainder(*this); // At each step, n = d * q + r

	while (remainder != Polynomial(0) && remainder.GetDegree() >= mod.GetDegree())
	{
		Polynomial temp = Polynomial(remainder.coefficients[0] / mod.coefficients[0]);
		quotient = quotient + temp;
		remainder = remainder - temp * mod;

	}

	return remainder;
}

// Two polynomials are equal if every corresponding coefficient is equal.
bool Polynomial::operator==(const Polynomial& right) const
{
	if (GetDegree() != right.GetDegree())
		return false;

	for (int i = 0; i < GetDegree(); i++)
	{
		if (std::abs(coefficients[i] - right.coefficients[i]) < EPSILON)
			return false;
	}

	return true;
}

bool Polynomial::operator!=(const Polynomial& right) const
{
	return !(this->operator==(right));
}

// Define an ordering on Polynomials. If p(x) has higher degree than q(x), then we say it's greater.
// If they have the same degree, then start from the coefficient of the highest degree term, and compare corresponding values. 
bool Polynomial::operator>(const Polynomial& right) const
{
	if (GetDegree() > right.GetDegree())
		return true;
	
	if (GetDegree() < right.GetDegree())
		return false;

	for (int i = GetDegree(); i >= 0; i--)
	{
		if (coefficients[i] > right.coefficients[i])
			return true;
		
		if (coefficients[i] < right.coefficients[i])
			return false;
	}

	// Only happens if they're equal
	return false;
}

bool Polynomial::operator<(const Polynomial& right) const
{
	if (GetDegree() > right.GetDegree())
		return false;

	if (GetDegree() < right.GetDegree())
		return true;

	for (int i = GetDegree(); i >= 0; i--)
	{
		if (coefficients[i] > right.coefficients[i])
			return false;

		if (coefficients[i] < right.coefficients[i])
			return true;
	}

	// Only happens if they're equal
	return false;
}

int Polynomial::GetDegree() const
{
	return coefficients.size() - 1;
}

std::string Polynomial::ToString() const
{
	std::stringstream displayStr;

	for (int i = GetDegree(); i >= 0; i--)
	{
		float coeff = coefficients[i];
		int precision = 4;
		
		if (coeff == 0 && GetDegree() != 0)
			continue;

		// If the coefficient is close enough to being an integer, don't show any decimal points
		if (std::abs(roundf(coeff) - coeff) < 0.00001)
			precision = 0;

		if(i != GetDegree())
			displayStr << " + ";

		if (i == 0)
		{	
			displayStr << std::setprecision(precision) << coeff;
		}
		else if (i == 1)
		{
			if(coefficients[i] == 1)
				displayStr << "x";
			else
				displayStr << std::setprecision(precision) << coeff << "x";
		}
		else
			if (coeff == 1)
				displayStr << "x^" << std::to_string(i);
			else
				displayStr << std::setprecision(precision) << coeff << "x^" << std::to_string(i);
	}

	return displayStr.str();
}