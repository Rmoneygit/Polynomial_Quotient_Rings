// File: Polynomial.h
// Author: Ryan P.
// Timestamp: <30-Mar-2024> 22:15:22 PDT
// Purpose: Class which represents a polynomial

#pragma once
#include <string>
#include <vector>

class Polynomial
{
	public:
		Polynomial();
		Polynomial(int a);
		Polynomial(const std::vector<float>& values);
		~Polynomial();
		
		Polynomial operator+(const Polynomial& ref) const;
		Polynomial operator-(const Polynomial& ref) const;
		Polynomial operator*(const Polynomial& ref) const;
		Polynomial operator/(const Polynomial& ref) const;
		Polynomial operator%(const Polynomial& ref) const;
		Polynomial operator-() const;
		bool operator==(const Polynomial& ref) const;
		bool operator!=(Polynomial ref) const;

		int GetDegree() const;

		std::string ToString() const;

		friend struct PolynomialHash;
	private:
		// Coefficient are stored backwards from how they are written on paper. For instance
		// 
		//				5x^3 + 2x^2 + 8x + 2
		// 
		// would be stored as [2, 8, 2, 5]. 
		// This has the advantage that the nth number in the vector is the coefficient of x^n.
		std::vector<float> coefficients;
};

struct PolynomialHash
{
	size_t operator()(const Polynomial& p) const
	{
		size_t h = p.coefficients[0];

		for (int i = 1; i < p.coefficients.size(); i++)
		{
			h = h ^ std::hash<float>()(p.coefficients[i]);
		}

		return h;
	}
};
