#ifndef __CODE_H__
#define __CODE_H__

#include "circle.h"

const int MAX_CODE_SIZE = 360;

int getBinIndex(double alpha, int total);

class CountCode {
	int size;
	int summation;
	int code[MAX_CODE_SIZE];
public:
	CountCode():size(MAX_CODE_SIZE),summation(0){}
	CountCode(int s):size(s){}
	double distance(const CountCode & rh) const;
	double rotateDistance(const CountCode & rh, int offset = 0) const;
	double emDistance(const CountCode & rh) const;
	double emRotateDistance(const CountCode & rh, int offset = 0) const;
	void fromCircle(const Circle & circle);
	void obfusticate();
	inline int getAt(int i)const{return code[i];}
	inline int getSize()const{return size;}
	inline int getSum()const{return summation;}
};

#endif
