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

const std::string digits = "0123456789";
bool isDigit(char c);

int main()
{
    std::string inputtedModulus;
    std::cout << "Enter a polynomial modulus. Type it like this: \"3 0 2 3 0\" represents the polynomial 3x^4 + 2x^2 + 3x.\n";
    std::getline(std::cin, inputtedModulus);

    // The goal here is to parse a string which is a list of floats seperated by spaces.
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
            float coeff = std::stof(substr);
            coefficients.push_back(coeff);
        }
    }

    // I want to enter the coefficients in the same order that they'd be written on paper, but ultimately store them reversed.
    std::reverse(coefficients.begin(), coefficients.end());
    Polynomial mod(coefficients);

    std::cout << mod.ToString();

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