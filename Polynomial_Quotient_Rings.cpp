// File: Polynomial_Quotient_Rings.cpp
// Author: Ryan P.
// Timestamp: <30-Mar-2024> 19:43:31 PDT
// Purpose: Main driver for a program which prints arithmetic tables for polynomial quotient rings.

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <string>
#include "Polynomial.h"
#include <vector>
#include <set>

const std::string digits = "0123456789";
bool isDigit(char c);
void generatePolynomialSet(int degree, std::set<Polynomial>& polynomials, float minCoeff, float maxCoeff, float step);
void parseListOfRealNumbers(std::string str, std::vector<float>& numbers);

int main()
{
    std::string inputtedModulus;
    std::cout << "We will assume the field of coefficients is the real numbers.\n";
    std::cout << "Enter a polynomial modulus. Type it like this: \"3 0 2 3 0\" represents the polynomial 3x^4 + 2x^2 + 3x.\n";
    std::getline(std::cin, inputtedModulus);

    std::vector<float> coefficients;
    parseListOfRealNumbers(inputtedModulus, coefficients);

    std::string inputtedMinMaxAndStep;
    std::cout << "Enter the minimum coefficient, maximum coefficient, and step value seperated by spaces.\n";
    std::getline(std::cin, inputtedMinMaxAndStep);

    std::vector<float> minMaxAndStep;
    parseListOfRealNumbers(inputtedMinMaxAndStep, minMaxAndStep);

    // I want to enter the coefficients in the same order that they'd be written on paper, but ultimately store them reversed.
    std::reverse(coefficients.begin(), coefficients.end());
    Polynomial mod(coefficients);

    std::set<Polynomial> polynomials;

    if (minMaxAndStep.size() == 3)
        generatePolynomialSet(mod.GetDegree(), polynomials, minMaxAndStep[0], minMaxAndStep[1], minMaxAndStep[2]);
    else
        std::cout << "Expected 3 values to be inputted, instead received " << minMaxAndStep.size() << std::endl;

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

void generatePolynomialSet(int degree, std::set<Polynomial>& polynomials, float minCoeff, float maxCoeff, float step)
{
    if (minCoeff > maxCoeff)
    {
        std::cout << "The minimum coefficient must be smaller than the maximum coefficient!\n";
        return;
    }

    if (step < 0)
    {
        std::cout << "The step value must be a positive real number!\n";
        return;
    }
    
    if (degree == 0)
    {
        for (float i = minCoeff; i <= maxCoeff; i = i + step)
        {
            polynomials.insert(Polynomial(i));
        }

        return;
    }

    std::set<Polynomial> lowerDegreePolynomials;
    generatePolynomialSet(degree - 1, lowerDegreePolynomials, minCoeff, maxCoeff, step);

    for (float i = minCoeff; i <= maxCoeff; i = i + step)
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

// The goal here is to parse a string which is a list of real numbers seperated by spaces.
void parseListOfRealNumbers(std::string str, std::vector<float>& numbers)
{
    for (int i = 0; i < str.size(); i++)
    {
        std::string substr;

        while (i < str.size())
        {
            if (str[i] == ' ')
            {
                break;
            }
            else if (isDigit(str[i]) || str[i] == '.')
            {
                substr += str[i];
                ++i;
            }
        }

        if (!substr.empty())
        {
            float coeff = std::stof(substr);
            numbers.push_back(coeff);
        }
    }
}