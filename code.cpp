#include <cmath>
#include "code.h"

inline int getBinIndex(double alpha, int total)
{
	return total*alpha/(2*M_PI);
}

double CountCode::distance(const CountCode & rh) const
{
	int sum = 0;
	for(int i = 0; i < size; i++)
		sum += abs(code[i]-rh.code[i]);
	return (double)sum/size;
}

void CountCode::fromCircle(const Circle & circle)
{
	for(int i = 0; i < size; i++)
		code[i] = 0;
	for(int i = 0; i < circle.getSize(); i++)
	{
		double a = circle.getPoint(i);
		while(a >= M_PI*2) a -= M_PI*2;
		while(a < 0) a += M_PI*2;
		int index = getBinIndex(a,size);
		code[index]++;
	}
}
