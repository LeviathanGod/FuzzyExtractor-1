#include <iostream>
#include <opencv2/opencv.hpp>
#include "minutiae_template.h"
#include "circle.h"

const int TEMPLATE_SIZE = 10;
const int MAX_X = 255;
const int MAX_Y = 255;

using namespace std;
using namespace cv;

int main()
{
	srand((unsigned)time(0));
	MinutiaeTemplate::setMaxXY(MAX_X,MAX_Y);
	MinutiaeTemplate mTemplate;
	mTemplate.fillWithRandom(TEMPLATE_SIZE);
	Circle c;
	c.getCenter(mTemplate);
	c.fromTemplate(mTemplate);
	int w = 500, h = 500, offsetx = 100, offsety = 100;
	Mat mat = Mat(w, h, CV_8UC3, Scalar(255, 255, 255));
	for(int i = 0; i < mTemplate.getSize(); i++)
	{
		int x = mTemplate.getMinutia(i).getX();
		int y = mTemplate.getMinutia(i).getY();
		cout << x << ' ' << y << endl;
		circle(mat,Point(x+offsetx,y+offsety),2,Scalar(0,0,255));
	}
	cout << endl;
	cout << "center: " << c.getCenterX() << ' ' << c.getCenterY() << endl;
	circle(mat,Point(c.getCenterX()+offsetx,c.getCenterY()+offsety),2,Scalar(0,255,0));
	int R = c.getR();
	cout << "R: " << R << endl;
	circle(mat,Point(c.getCenterX()+offsetx,c.getCenterY()+offsety),R,Scalar(0,255,0));
	for(int i = 0; i < c.getSize(); i++)
	{
		double a = c.getPoint(i);
		cout << a << endl;
		circle(mat,Point(c.getCenterX()+R*cos(a)+offsetx,c.getCenterY()+R*sin(a)+offsety),2,Scalar(255,0,0));
	}
	imshow("img",mat);
	waitKey();
	return 0;
}
