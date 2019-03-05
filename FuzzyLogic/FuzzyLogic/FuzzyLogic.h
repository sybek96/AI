#ifndef _FUZZY_LOGIC_H
#define _FUZZY_LOGIC_H

#include <algorithm>

class FuzzyLogic
{
public:
	static double FuzzyAND(double a, double b);
	static double FuzzyOR(double a, double b);
	static double FuzzyNOT(double a);
protected:
private: 
	FuzzyLogic() = default;

};

#endif // !_FUZZY_LOGIC_H
