#ifndef __CODE_H__
#define __CODE_H__

#include "circle.h"

const int MAX_CODE_SIZE = 360;

int getBinIndex(double alpha, int total);

class CountCode {
	int size;
	int code[MAX_CODE_SIZE];
public:
	CountCode(int s):size(s){}
	double distance(const CountCode & rh) const;
	void fromCircle(const Circle & circle);
};

#endif
