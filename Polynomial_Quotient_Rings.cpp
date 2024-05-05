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

int main()
{
    std::string inputtedModulus;
    std::cout << "Enter a polynomial modulus. Type it like this: \"3 0 2 3 0\" represents the polynomial 3x^4 + 2x^2 + 3x.\n";
    std::cin >> inputtedModulus;

    std::vector<float> coefficients;
    for (std::string::iterator it = inputtedModulus.begin(); it != inputtedModulus.end(); ++it)
    {
        std::string substr;

        while (it != inputtedModulus.end())
        {
            if (*it == ' ')
            {
                float coeff = std::stof(substr);
                coefficients.push_back(coeff);
                break;
            }
            else
            {
                substr += *it;
                ++it;
            }
        }
    }

    // I want to enter the coefficients in the same order that they'd be written on paper, but ultimately store them reversed.
    std::reverse(coefficients.begin(), coefficients.end());
    Polynomial mod(coefficients);

    return 0;
}