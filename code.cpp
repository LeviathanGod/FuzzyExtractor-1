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
	return ((double)sum)/size;
}

double CountCode::rotateDistance(const CountCode & rh, int offset) const
{
	int min = 0x7fffffff;
	for(int i = -offset; i < offset; i++)
	{
		int sum = 0;
		for(int j = 0; j < size; j++)
			sum += abs(code[j]-rh.code[(j+i+size)%size]);
		if(min > sum) min = sum;
	}
	return ((double)min)/size;
}

double CountCode::emDistance(const CountCode & rh) const
{
	int curr = 0;
	double sum = 0;
	for(int i = 0; i < size; i++)
		sum += abs((curr+=(code[i]-rh.code[i]),curr));
	return ((double)sum)/size;
}

double CountCode::emRotateDistance(const CountCode & rh, int offset) const
{
	int min = 0x7fffffff;
	for(int i = -offset; i < offset; i++)
	{
		int curr = 0;
		int sum = 0;
		for(int j = 0; j < size; j++)
			sum += abs((curr+=(code[j]-rh.code[(i+j+size)%size]),curr));
		if(min > sum) min = sum;
	}
	return ((double)min)/size;
}

void CountCode::fromCircle(const Circle & circle)
{
	for(int i = 0; i < size; i++)
		code[i] = 0;
	summation = 0;
	for(int i = 0; i < circle.getSize(); i++)
	{
		double a = circle.getPoint(i);
		while(a >= M_PI*2) a -= M_PI*2;
		while(a < 0) a += M_PI*2;
		int index = getBinIndex(a,size);
		code[index]++;
		summation++;
	}
}

void CountCode::obfusticate()
{
	int ncode[MAX_CODE_SIZE];
	for(int i = 0; i < size; i++)
		ncode[i] = (code[i]+code[(i+size-1)%size]+code[(i+1)%size])/3;
	for(int i = 0; i < size; i++)
		code[i] = ncode[i];
}
