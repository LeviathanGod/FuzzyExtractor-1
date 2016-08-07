#include <cmath>
#include <cassert>
#include <iostream>
#include "circle.h"
#include "minutiae_template.h"

double getAngle(double x, double y)
{
	if(x == 0)
	{
		if(y > 0) return M_PI/2;
		return -M_PI/2;
	}
	double t = atan(y/x);
	if(t < 0) t += M_PI;
	if(y < 0) return t+M_PI;
	return t;
}

inline double bound(double a)
{
	if(a > 2 * M_PI) return a - 2 * M_PI;
	if(a < 0) return a + 2 * M_PI;
	return a;
}

void Circle::getCenter(const MinutiaeTemplate & mt){
	if(mt.getSize() == 0) return;
	int sumX = 0, sumY = 0, number = mt.getSize();
	for(int i = 0; i < number; i++)
	{
		sumX += mt.getMinutia(i).getX();
		sumY += mt.getMinutia(i).getY();
	}
	center[0] = (sumX+number/2)/number;
	center[1] = (sumY+number/2)/number;
}

void Circle::fromMinutiaPair(const Minutia & m1, const Minutia & m2)
{
	if(distance(m1,m2) <= T) return;
	double x1 = m1.getX(), y1 = m1.getY();
	double x2 = m2.getX(), y2 = m2.getY();
	double x0 = center[0], y0 = center[1];
	double dx1 = x1-x0, dy1 = y1-y0;
	double dx2 = x2-x0, dy2 = y2-y0;
	double rr1 = dx1*dx1+dy1*dy1;
	double a,phi;
	if(rr1 > 0)
	{
		double fx = (dx1*dx2+dy1*dy2)/rr1;
		double fy = (dx1*dy2-dy1*dx2)/rr1;
		double fx1 = fx-1;
		double t;
		if(fx1 == 0) {a = 0; t = M_PI/2;}
		else
		{
			t = atan(fy/fx1);
			if(t < 0) t += M_PI;
			a = t-M_PI/2+getAngle(dx1,dy1);
		}
		double d = sin(t)*sqrt(rr1);
		if(d > R) return;
		phi = acos(d/R);
		assert(!isnan(phi));
		assert(!isnan(a));
	}
	else
	{
		a = getAngle(dx2,dy2)+M_PI/2;
		phi = M_PI/2;
		assert(!isnan(phi));
		assert(!isnan(a));
	}
	points.push_back(bound(a+phi));
	points.push_back(bound(a-phi));
}

void Circle::fromTemplate(const MinutiaeTemplate & mt)
{
	points.clear();
	for(int i = 0; i < mt.getSize(); i++)
		for(int j = i+1; j < mt.getSize(); j++)
			fromMinutiaPair(mt.getMinutia(i),mt.getMinutia(j));
}
