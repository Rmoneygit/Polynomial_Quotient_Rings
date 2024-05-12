// File: Polynomial.h
// Author: Ryan P.
// Timestamp: <30-Mar-2024> 22:21:34 PDT
// Purpose: Class which represents a polynomial

#include <iomanip>
#include <stdexcept>

#include "Polynomial.h"

Polynomial::Polynomial()
{
	coefficients.push_back(0);
}

Polynomial::Polynomial(int a)
{
	coefficients.push_back(a);
}

Polynomial::Polynomial(const std::vector<int>& values)
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
	std::vector<int> newCoefficients;

	int higherDegree = GetDegree();
	if (right.GetDegree() > higherDegree)
		higherDegree = right.GetDegree();

	// Add corresponding coefficients.
	for (int i = 0; i <= higherDegree; i++)
	{
		int leftCoefficent = 0;
		int rightCoefficient = 0;

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
	std::vector<int> newCoefficients;

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
	std::vector<int> newCoefficients;

	int maximumNewDegree = GetDegree() + right.GetDegree();

	// Formula for the nth coefficient of the product of two arbitrary polynomials:
	// c_n = a_0*b_n + a_1*b_(n-1) a_2*b_(n-2) + a_3*b_(n-3) + ... + a_(n-1)b_1 + a_n*b_0
	for (int i = 0; i < maximumNewDegree; i++)
	{
		int ithCoeff = 0;
		
		for (int j = i; j >= 0; j--)
		{
			int leftCoefficent = 0;
			int rightCoefficient = 0;

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
		if (coefficients[i] != right.coefficients[i])
			return false;
	}

	return true;
}

bool Polynomial::operator!=(Polynomial right) const
{
	return !(this->operator==(right));
}

int Polynomial::GetDegree() const
{
	return coefficients.size() - 1;
}

std::string Polynomial::ToString()
{
	std::string displayStr;

	for (int i = GetDegree(); i >= 0; i--)
	{
		if (coefficients[i] == 0 && GetDegree() != 0)
			continue;

		if(i != GetDegree())
			displayStr += " + ";

		if (i == 0)
		{	
			displayStr += std::to_string(coefficients[i]);
		}
		else if (i == 1)
		{
			displayStr += std::to_string(coefficients[i]) + "x";
		}
		else
			displayStr += std::to_string(coefficients[i]) + "x^" + std::to_string(i);
	}

	return displayStr;
}