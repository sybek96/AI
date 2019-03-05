#ifndef _FUZZY_MEMBERSHIP_H
#define _FUZZY_MEMBERSHIP_H

class FuzzyMembership
{
public:
	static double FuzzyGradeDown(double value, double x0, double x1);
	static double FuzzyGradeUp(double value, double x0, double x1);
	static double FuzzyTrapezoid(double value, double x0, double x1, double x2, double x3);
protected:   
private:  
	FuzzyMembership() = default;
};

#endif // !_FUZZY_MEMBERSHIP_H
