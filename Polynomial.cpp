// File: Polynomial.h
// Author: Ryan P.
// Timestamp: <30-Mar-2024> 22:21:34 PDT
// Purpose: Class which represents a polynomial

#include "Polynomial.h"
#include <stdexcept>

Polynomial::Polynomial()
{
	coefficients.push_back(0);
}

Polynomial::Polynomial(int a)
{
	coefficients.push_back(a);
}

Polynomial::Polynomial(const std::vector<float>& values)
{
	for (int val : values)
	{
		coefficients.push_back(val);
	}
}

Polynomial::~Polynomial()
{

}

Polynomial Polynomial::operator+(Polynomial right)
{
	std::vector<float> newCoefficients;

	int higherDegree = getDegree();
	if (right.getDegree() > higherDegree)
		higherDegree = right.getDegree();

	// Add corresponding coefficients.
	for (int i = 0; i < higherDegree; i++)
	{
		int leftCoefficent = 0;
		int rightCoefficient = 0;

		if (i < getDegree())
			leftCoefficent = coefficients[i];

		if (i < right.getDegree())
			rightCoefficient = right.coefficients[i];

		newCoefficients.push_back(leftCoefficent + rightCoefficient);
	}

	// Remove zeroes from the end of the coefficient list
	for (int i = newCoefficients.size() - 1; i >= 0;)
	{
		if (newCoefficients[i] == 0)
			newCoefficients.pop_back();
	}

	return Polynomial(newCoefficients);
}

// Get the additive inverse of this polynomial. Aka, just multiply it by -1.
Polynomial Polynomial::operator-()
{
	std::vector<float> newCoefficients;

	for (int i = 0; i < getDegree(); i++)
	{
		newCoefficients.push_back(-coefficients[i]);
	}

	return Polynomial(newCoefficients);
}

Polynomial Polynomial::operator-(Polynomial right)
{
	return this->operator+(-right);
}

Polynomial Polynomial::operator*(Polynomial right)
{
	std::vector<float> newCoefficients;

	int maximumNewDegree = getDegree() + right.getDegree();

	// Formula for the nth coefficient of the product of two arbitrary polynomials:
	// c_n = a_0*b_n + a_1*b_(n-1) a_2*b_(n-2) + a_3*b_(n-3) + ... + a_(n-1)b_1 + a_n*b_0
	for (int i = 0; i < maximumNewDegree; i++)
	{
		int ithCoeff = 0;
		
		for (int j = i; j >= 0; j--)
		{
			int leftCoefficent = 0;
			int rightCoefficient = 0;

			if (i < getDegree())
				leftCoefficent = coefficients[i];

			if (i < right.getDegree())
				rightCoefficient = right.coefficients[j];
			
			ithCoeff += leftCoefficent * rightCoefficient;
		}

		newCoefficients.push_back(ithCoeff);
	}

	// Remove zeroes from the end of the coefficient list
	for (int i = newCoefficients.size() - 1; i >= 0;)
	{
		if (newCoefficients[i] == 0)
			newCoefficients.pop_back();
	}

	return Polynomial(newCoefficients);
}

Polynomial Polynomial::operator/(Polynomial divisor)
{
	if (divisor == Polynomial(0))
	{
		throw std::invalid_argument("Division by 0 is not defined.");
	}
	
	Polynomial quotient(0);
	Polynomial remainder(*this); // At each step, n = d * q + r

	while (remainder != Polynomial(0) && remainder.getDegree() >= divisor.getDegree())
	{
		Polynomial temp = Polynomial(remainder.coefficients[0] / divisor.coefficients[0]);
		quotient = quotient + temp;
		remainder = remainder - temp * divisor;

	}

	return quotient;
}

// This operator should give us the least residue modulo "mod"
// For polynomial rings over a field, this happens to be the remainder upon division by the modulus.
Polynomial Polynomial::operator%(Polynomial mod)
{
	if (mod == Polynomial(0))
	{
		// Since f(x) - f(x) = 0, then
		// f(x) is congruent to f(x) mod 0. 
		return *this;
	}

	Polynomial quotient(0);
	Polynomial remainder(*this); // At each step, n = d * q + r

	while (remainder != Polynomial(0) && remainder.getDegree() >= mod.getDegree())
	{
		Polynomial temp = Polynomial(remainder.coefficients[0] / mod.coefficients[0]);
		quotient = quotient + temp;
		remainder = remainder - temp * mod;

	}

	return remainder;
}

// Two polynomials are equal if every corresponding coefficient is equal.
bool Polynomial::operator==(Polynomial right)
{
	if (getDegree() != right.getDegree())
		return false;

	for (int i = 0; i < getDegree(); i++)
	{
		if (coefficients[i] != right.coefficients[i])
			return false;
	}

	return true;
}

bool Polynomial::operator!=(Polynomial right)
{
	return !(this->operator==(right));
}

int Polynomial::getDegree()
{
	return coefficients.size();
}