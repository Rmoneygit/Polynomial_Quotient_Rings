// File: Polynomial_Quotient_Rings.cpp
// Author: Ryan P.
// Timestamp: <30-Mar-2024> 19:43:31 PDT
// Purpose: Main driver for a program which prints arithmetic tables for polynomial quotient rings.

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include "Polynomial.h"
#include <vector>
#include <unordered_set>

const std::string digits = "0123456789";
bool isDigit(char c);

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

void generatePolynomialSet(int degree, std::unordered_set<Polynomial, PolynomialHash>& polynomials, float minCoeff, float maxCoeff);

int main()
{
    std::string inputtedModulus;
    std::cout << "We will assume the field of coefficient is the real numbers.\n";
    std::cout << "Enter a polynomial modulus. Type it like this: \"3 0 2 3 0\" represents the polynomial 3x^4 + 2x^2 + 3x.\n";
    std::getline(std::cin, inputtedModulus);

    // The goal here is to parse a string which is a list of integers seperated by spaces.
    std::vector<float> coefficients;
    for (int i = 0; i < inputtedModulus.size(); i++)
    {
        std::string substr;

        while (i < inputtedModulus.size())
        {
            if (inputtedModulus[i] == ' ')
            {
                break;
            }
            else if(isDigit(inputtedModulus[i]) || inputtedModulus[i] == '.')
            {
                substr += inputtedModulus[i];
                ++i;
            }
        }

        if (!substr.empty())
        {
            int coeff = std::stof(substr);
            coefficients.push_back(coeff);
        }
    }

    // I want to enter the coefficients in the same order that they'd be written on paper, but ultimately store them reversed.
    std::reverse(coefficients.begin(), coefficients.end());
    Polynomial mod(coefficients);

    std::unordered_set<Polynomial, PolynomialHash> polynomials;
    generatePolynomialSet(mod.GetDegree(), polynomials, 0, 2);

    for (Polynomial p : polynomials)
    {
        std::cout << p.ToString() << std::endl;
    }

    return 0;
}

bool isDigit(char c)
{
    for (int i = 0; i < digits.size(); i++)
    {
        if (c == digits[i])
            return true;
    }

    return false;
}

void generatePolynomialSet(int degree, std::unordered_set<Polynomial, PolynomialHash>& polynomials, float minCoeff, float maxCoeff)
{
    if (degree == 0)
    {
        for (int i = minCoeff; i <= maxCoeff; ++i)
        {
            polynomials.insert(Polynomial({ i }));
        }

        return;
    }

    std::unordered_set<Polynomial, PolynomialHash> lowerDegreePolynomials;
    generatePolynomialSet(degree - 1, lowerDegreePolynomials, minCoeff, maxCoeff);

    for (int i = minCoeff; i <= maxCoeff; ++i)
    {   
        // Create a single-term polynomial like 2x^2
        std::vector<float> coeffs;
        for (int j = 0; j <= degree; ++j)
        {
            if (j == degree)
                coeffs.push_back(i);
            else
                coeffs.push_back(0);
        }

        Polynomial p(coeffs);
        
        for (Polynomial q : lowerDegreePolynomials)
        {
            polynomials.insert(p + q);
        }
    }

    polynomials.merge(lowerDegreePolynomials);
}