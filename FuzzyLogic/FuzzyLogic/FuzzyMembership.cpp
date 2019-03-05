#include "FuzzyMembership.h"
/// <summary>
/// Used for line that goes downward on graph
/// </summary>
/// <param name="value">value of the member</param>
/// <param name="x0">cutoff below which membership is 1</param>
/// <param name="x1">cutoff above which membership is 0</param>
/// <returns>membership degree within set</returns>
double FuzzyMembership::FuzzyGradeDown(double value, double x0, double x1)
{
	double result = 0;
	double x = value;

	if (x <= x0)
	{
		result = 1;
	}
	else if (x > x1)
	{
		result = 0;
	}
	else
	{
		result = ((x1 - x) / (x1 - x0)); //down proportion
	}

	return result;
}

/// <summary>
/// Used for line that goes up on the graph
/// </summary>
/// <param name="value">value of member</param>
/// <param name="x0">cutoff below which degree is 0</param>
/// <param name="x1">cutoff above which degree is 1</param>
/// <returns></returns>
double FuzzyMembership::FuzzyGradeUp(double value, double x0, double x1)
{
	double result = 0;
	double x = value;

	if (x <= x0)
	{
		result = 0;
	}
	else if (x > x1)
	{
		result = 1;
	}
	else
	{
		result = ((x - x0) / (x1 - x0)); //up proportion
	}

	return result;
}

/// <summary>
/// Used for trapezoids on the graph
/// </summary>
/// <param name="value">value of the member</param>
/// <param name="x0">value below which degree is 0</param>
/// <param name="x1">cutoff above which degree is 1</param>
/// <param name="x2">cutoff below which degree is 1</param>
/// <param name="x3">value above which degree is 0</param>
/// <returns></returns>
double FuzzyMembership::FuzzyTrapezoid(double value, double x0, double x1, double x2, double x3)
{
	double result = 0;
	double x = value;

	if ((x <= x0) || (x >= x3))
	{
		result = 0;
	}
	else if ((x >= x1) && (x <= x2))
	{
		result = 1;
	}
	else if ((x > x0) && (x < x1))
	{
		result = ((x - x0) / (x1 - x0));
	}
	else
	{
		result = ((x3 - x) / (x3 - x2));
	}
	return result;
}
