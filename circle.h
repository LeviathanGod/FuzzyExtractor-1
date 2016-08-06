#ifndef __CIRCLE_H__
#define __CIRCLE_H__

#include <vector>
#include "minutiae_template.h"

const int DEFAULT_R = 150;
const double DEFAULT_T = 0.0;

class Circle {
	int R;
	double T;
	int center[2];
	std::vector<double> points;
public:
	Circle():R(DEFAULT_R),T(DEFAULT_T) {center[0]=0;center[1]=0;}
	inline void setT(double t){T=t;}
	inline void setRadius(int r){R=r;}
	inline int getSize() const {return points.size();}
	inline double getPoint(int i) const {return points.at(i);}
	void getCenter(const MinutiaeTemplate & mt);
	inline int getCenterX() const {return center[0];}
	inline int getCenterY() const {return center[1];}
	inline int getR() const {return R;}
	void fromTemplate(const MinutiaeTemplate & mt);
	void fromMinutiaPair(const Minutia & m1, const Minutia & m2);
};

#endif
