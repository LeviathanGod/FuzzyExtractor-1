#include <cmath>
#include <algorithm>
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
	if(mt.getFilename())
	{
		center[0] = mt.getCenterX();
		center[1] = mt.getCenterY();
		return;
	}
	if(mt.getSize() == 0) return;
	int sumX = 0, sumY = 0, number = mt.getSize();
	for(int i = 0; i < number; i++)
	{
		sumX += mt.getMinutia(i).getX();
		sumY += mt.getMinutia(i).getY();
	}
	double wX = (double)sumX/number;
	double wY = (double)sumY/number;
	center[0] = (sumX+number/2)/number;
	center[1] = (sumY+number/2)/number;
}

void Circle::fromMinutiaPair(const Minutia & m1, const Minutia & m2)
{
	if(m1.distance(m2) <= T) return;
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

void Circle::getDistanceFromMinutiaPair(const Minutia & m1, const Minutia & m2)
{
	if(m1.distance(m2) <= T) return;
#if 1
	double x1 = m1.getX(), y1 = m1.getY();
	double x2 = m2.getX(), y2 = m2.getY();
	double x0 = center[0], y0 = center[1];
	double dx1 = x1-x0, dy1 = y1-y0;
	double dx2 = x2-x0, dy2 = y2-y0;
	// double rr1 = dx1*dx1+dy1*dy1;
	double l = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
	double d = fabs(dx1*(y2-y1)-dy1*(x2-x1))/l;
	if(d > R) return;
	double phi = acos(d/R);
	assert(!isnan(phi));

#if 0
	double D = sqrt(((x1+x2)/2-x0)*((x1+x2)/2-x0)+((y1+y2)/2-y0)*((y1+y2)/2-y0)-d*d);
	double phi2;
	if(D+l/2 < R)
	{
		double b;
		double a = acos((D+l/2)/R);
		if(D > l/2)
			b = acos((D-l/2)/R);
		else
			b = M_PI-acos((l/2-D)/R);
		phi2 = b-a;
		points.push_back(phi2);
	}
#endif
	points.push_back(bound(2*phi));
#else
	long x1 = m1.getX(), y1 = m1.getY();
	long x2 = m2.getX(), y2 = m2.getY();
	long x0 = center[0], y0 = center[1];
	long vx = y1-y2, vy = x2-x1;
	long delta1 = (vx*vx+vy*vy)*R*R+2*vx*vy*(x1-x0)*(y1-y0);
	long delta2 = (vx*vx+vy*vy)*R*R+2*vx*vy*(x2-x0)*(y2-y0);
	if(delta1 < 0 || delta2 < 0) return;
	long b1 = vx*(x0-x1)+vy*(y0-y1), b2 = vx*(x0-x2)+vy*(y0-y2);
	long a = vx*vx+vy*vy;
	if(b1*b1 > delta1 || b2*b2 > delta2) return;
	double l1 = fabs(abs(b1)-sqrt(delta1))/a;
	double l2 = fabs(abs(b2)-sqrt(delta2))/a;
	long xx1 = x1 + l1*vx;
	long yy1 = y1 + l1*vy;
	long xx2 = x2 + l2*vx;
	long yy2 = y2 + l2*vy;
	long dx = xx1-xx2, dy = yy1 - yy2;
	double l = sqrt(dx*dx+dy*dy);
	points.push_back(2*asin(l/(2*R)));
#endif
}

void Circle::getAnglesFromMinutiaPair(const Minutia & m1, const Minutia & m2)
{
	if(m1.distance(m2) <= T) return;
	double x1 = m1.getX(), y1 = m1.getY();
	double x2 = m2.getX(), y2 = m2.getY();
	double x0 = center[0], y0 = center[1];
	double dx1 = x1-x0, dy1 = y1-y0;
	double dx2 = x2-x0, dy2 = y2-y0;
	double dx = x1-x2, dy = y1-y2;
	double l1 = sqrt(dx1*dx1+dy1*dy1);
	double l2 = sqrt(dx2*dx2+dy2*dy2);
	double ll = sqrt(dx*dx+dy*dy);
	double a1 = acos((dx1*dx+dy1*dy)/(l1*ll));
	double a2 = acos(-(dx2*dx+dy2*dy)/(l2*ll));
	points.push_back(a1);
	points.push_back(a2);
}

void Circle::getDistanceSetFromTemplate(const MinutiaeTemplate & mt)
{
	points.clear();
	for(int i = 0; i < mt.getSize(); i++)
		for(int j = i+1; j < mt.getSize(); j++)
			getDistanceFromMinutiaPair(mt.getMinutia(i),mt.getMinutia(j));
}

void Circle::getAnglePairsFromTemplate(const MinutiaeTemplate & mt)
{
	points.clear();
	for(int i = 0; i < mt.getSize(); i++)
		for(int j = i+1; j < mt.getSize(); j++)
			getAnglesFromMinutiaPair(mt.getMinutia(i),mt.getMinutia(j));
}

void Circle::fromTemplate(const MinutiaeTemplate & mt)
{
	points.clear();
	for(int i = 0; i < mt.getSize(); i++)
		for(int j = i+1; j < mt.getSize(); j++)
			fromMinutiaPair(mt.getMinutia(i),mt.getMinutia(j));
	if(mt.getFilename())
	{
		for(int i = 0; i < points.size(); i++)
		{
			points[i] -= mt.getCenterAlpha();
			if(points[i] < 0) points[i] += 2*M_PI;
		}
	}
}

double Circle::distance(Circle & rh)
{
	sort();
	rh.sort();
	int i1 = 0, i2 = 0; double sum = 0;
	while(i1 < getSize() && i2 < rh.getSize())
	{
		if(points.at(i1) < rh.points.at(i2) - EPSILON)
			i1++;
		else if(points.at(i1) > rh.points.at(i2) + EPSILON)
			i2++;
		else
		{
			sum += 1; //points.at(i1);
			i1++;
			i2++;
		}
	}
	return sum/getSize();
}
