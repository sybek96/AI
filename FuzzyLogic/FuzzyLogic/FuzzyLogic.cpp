#include "FuzzyLogic.h"

/// <summary>
/// And operator for the fuzzy set
/// Gets the min of the 2 values
/// </summary>
/// <param name="a">value in set a</param>
/// <param name="b">value in set b</param>
/// <returns>Result of the operator as a double</returns>
double FuzzyLogic::FuzzyAND(double a, double b)
{
	return std::min(a, b);
}

/// <summary>
/// Or operator for the fuzzy set
/// Gets the max of the 2 values
/// </summary>
/// <param name="a">value in set a</param>
/// <param name="b">value in set b</param>
/// <returns>Result of the operator as a double</returns>
double FuzzyLogic::FuzzyOR(double a, double b)
{
	return std::max(a,b);
}

/// <summary>
/// Not operator for the fuzzy set
/// Gets the compliment of the value
/// </summary>
/// <param name="a">value in set</param>
/// <returns>Result of the operator as a double</returns>
double FuzzyLogic::FuzzyNOT(double a)
{
	return 1.0 - a;
}
