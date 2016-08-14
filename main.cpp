#include <cstdio>
#include <iostream>
#include <iomanip>
#include <cmath>
#include <algorithm>
#include <cstring>
#include <opencv2/opencv.hpp>
#include "minutiae_template.h"
#include "fingerprint_reader.h"
#include "circle.h"
#include "code.h"

#define TEST_SET_1
#define TEST_SET_2
#define TEST_SET_3

//#define COMPUTE_GAR
#define NO_ROTATE_FIRST
#define NO_ROTATE_SECOND
//#define SHOW_TEMPLATE
//#define CALCULATE_DISTANCE

const int TEMPLATE_SIZE = MINUTIAE_BUF_SIZE;
const int MAX_X = 255;
const int MAX_Y = 255;
const int TEMPLATE_NUM = 800;
const int TEST_SET_SIZE = 20;
const int disturbScale = 5;
const double DEFAULT_T2 = 5.0;

const int FINGERPRINT_NUM = 100;
const int FINGERPRINT_SET_SIZE = 2;

using namespace std;
using namespace cv;

int referencePointX[] = {
35,115,211,218,149,164,115,101,170,120,75,116,101,107,209,133,163,114,109,83,
168,145,90,130,183,155,193,140,63,127,117,139,111,160,199,147,130,166,172,159,
173,170,149,190,156,127,125,103,79,133,136,139,111,94,162,178,161,166,158,170,
104,100,133,105,182,210,145,183,120,120,161,219,143,152,131,146,164,97,151,137,
117,166,181,130,167,152,164,119,185,77,116,131,158,115,103,126,157,161,89,145,
154,91,185,154,133,179,174,162,136,150,123,148,123,217,155,133,124,93,101,147,
112,154,127,86,72,193,113,136,215,192,93,119,167,161,133,92,136,134,132,106,
176,196,146,71,76,237,150,149,148,107,147,101,109,108,129,157,216,140,169,147,
151,184,127,135,101,133,98,137,134,161,163,138,149,99,129,152,145,123,121,103,
106,103,137,164,153,156,111,122,83,214,99,124,130,130,116,137,150,124,130,99
};
int referencePointY[] = {
329,296,362,356,187,206,304,272,263,306,370,362,254,254,205,260,258,298,543,292,
236,252,306,380,351,331,218,263,197,253,399,228,300,296,194,183,306,319,383,406,
219,222,278,269,219,235,159,217,297,248,329,344,310,328,280,167,206,121,262,272,
271,308,302,318,249,184,238,242,282,155,220,265,185,275,223,169,240,263,373,384,
302,256,328,230,116,114,279,339,158,237,259,268,209,210,244,205,274,345,266,302,
209,137,312,252,237,277,232,237,229,245,310,284,242,237,271,267,348,367,347,367,
185,210,280,300,138,235,266,260,303,173,200,183,140,367,267,321,234,263,164,168,
288,244,218,205,221,221,255,286,249,298,198,237,210,237,297,305,275,244,265,305,
201,221,176,208,213,288,231,200,263,345,213,232,196,227,344,214,395,257,319,272,
201,160,278,193,248,234,229,204,248,294,221,240,304,379,247,194,236,207,208,243
};
double referencePointA[] = {
0.0289553,0.0825433,5.98124,6.11597,0.0792138,0.0879944,6.05881,6.14181,6.11742,6.14039,
6.00924,6.04899,0.0352637,0.0451576,0.0516902,0.0615757,6.10357,6.12232,5.19747,5.71216,
0.0271895,0.190853,0.122276,0.0621611,0.491691,0.328681,0.23797,0.183899,0.00330305,6.21182,
6.07387,6.04046,6.03114,6.04155,0.0103944,6.26009,0.361116,0.20806,6.05392,5.93717,
0.242474,0.238123,6.25495,6.23438,0.11727,0.197151,6.08146,6.05774,0.0804525,0.0754655,
0.0933053,0.000621867,6.13486,6.10639,5.78924,5.96546,0.221451,0.174435,0.1518,0.142045,
6.06767,6.05145,5.83921,5.85621,0.116331,0.179304,0.389466,0.244301,0.211172,0.0973583,
0.262755,0.171406,5.99162,5.93869,6.23695,6.24814,6.13408,5.97932,5.78184,5.89095,
6.16806,6.22529,0.0423454,0.0615028,0.120957,0.0904392,0.0104688,0.0501259,6.16633,6.1158,
6.23233,6.11717,6.0817,6.12386,0.0988317,0.0696381,6.21015,6.08121,0.129137,0.151137,
0.174952,0.239312,6.00508,5.97449,0.0102272,6.26493,0.103554,0.134269,0.281297,0.106037,
6.01419,5.97697,0.108613,0.248423,0.227639,0.0990541,0.300987,0.309511,6.01631,5.93307,
0.181126,0.146767,5.96381,5.9256,6.17445,6.16842,5.94002,5.94661,0.247954,0.120812,
0.191536,0.214803,0.230795,0.0775644,6.02872,6.16359,0.0174377,0.0113986,6.09643,6.01916,
0.0945412,0.163494,0.0344496,0.00290239,6.05819,6.11693,6.21208,6.15977,6.18508,6.1566,
6.04744,5.98822,6.2383,6.22115,0.119529,0.00122885,0.101522,0.14853,0.0675504,0.0815498,
6.26723,6.23456,0.149314,0.150997,6.21599,6.26795,6.11618,6.17325,0.0302291,6.16283,
0.0398252,0.0499164,6.2729,6.2217,0.0838706,0.0932566,0.376317,0.243934,6.22316,6.17594,
0.069127,0.080778,0.15654,0.224985,0.130939,0.080093,6.17936,6.13312,0.0315114,0.0937427,
6.10063,6.13145,0.273534,0.0822335,6.23365,0.0154964,0.238743,0.200549,6.05042,6.13576
};

char filenames[FINGERPRINT_NUM][FINGERPRINT_SET_SIZE][100];

const char * path = "fingerprints/";
const char * getFullPath(char *buf, int i, int j)
{
	strcpy(buf,path);
	sprintf(buf+strlen(buf),"%d%d%d_%d.pgm",i/100,(i/10)%10,i%10,j);
	return buf;
}

void drawCode(const CountCode & code, int num)
{
	int w=800,h=400;
	Mat mat(h,w,CV_8UC3,Scalar(255,255,255));
	int vsize = h/50;
	int hsize = w/code.getSize();
	Point points[MAX_CODE_SIZE];
	for(int i = 0; i < code.getSize(); i++)
	{
		points[i] = Point(i*hsize,h - code.getAt(i)*vsize);
		circle(mat,points[i],3,Scalar(0,0,255),CV_FILLED);
	}
	for(int i = 0; i < code.getSize()-1; i++)
		line(mat,points[i],points[i+1],Scalar(0,0,0));
	char buf[10];
	sprintf(buf,"%d",num);
	imshow(buf,mat);
}

void drawCircle(const Circle & c1, const Circle & c2, int num)
{
	int w=800,h=400;
	Mat mat(h,w,CV_8UC3,Scalar(255,255,255));
	double points1[2000], points2[2000];
	for(int i = 0; i < c1.getSize(); i++) points1[i] = c1.getPoint(i);
	for(int i = 0; i < c2.getSize(); i++) points2[i] = c2.getPoint(i);
	sort(&points1[0],&points1[c1.getSize()]);
	sort(&points2[0],&points2[c2.getSize()]);
	double hsize = 1.8, vsize = 80;

	int i1 = 0, i2 = 0;
	while(i1 < c1.getSize()-1 || i2 < c2.getSize()-1)
	{
		int i = i1 > i2? i1: i2;
		if(i1 < c1.getSize()-1)
			line(mat,Point(i*hsize,h-points1[i1]*vsize),Point((i+1)*hsize,h-points1[i1+1]*vsize),Scalar(255,0,0));
		if(i2 < c2.getSize()-1)
			line(mat,Point(i*hsize,h-points2[i2]*vsize),Point((i+1)*hsize,h-points2[i2+1]*vsize),Scalar(0,0,255));
		if(i1 < c1.getSize()-1)
		{
			if(i2 >= c2.getSize()-1 || points2[i2] > points1[i1] + EPSILON) i1++;
			else if(points2[i2] < points1[i1] - EPSILON) i2++;
			else
			{
				line(mat,Point(i*hsize,h-points1[i1]*vsize+4),Point(i*hsize,h-points1[i1]*vsize-4),Scalar(0,0,0));
				i1++; i2++;
			}
		}
		else i2++;
	}

	char buf[10];
	sprintf(buf,"%d",num);
	imshow(buf,mat);
}

void showProcess(const MinutiaeTemplate & mt, double T, const char *name, const char *filename)
{
	Mat mat(700, 700, CV_8UC3, Scalar(255, 255, 255));
	Mat fingerprint = imread(filename);
	int offsetx = 200, offsety = 0, R = DEFAULT_R;
	fingerprint.copyTo(mat.rowRange(offsety,offsety+fingerprint.rows).colRange(offsetx,offsetx+fingerprint.cols));
	circle(mat,Point(mt.getCenterX()+offsetx,mt.getCenterY()+offsety),R,Scalar(0,255,0));
	circle(mat,Point(mt.getCenterX()+offsetx,mt.getCenterY()+offsety),3,Scalar(0,255,0),CV_FILLED);
	for(int i = 0; i < mt.getSize(); i++)
	{
		for(int j = i+1; j < mt.getSize(); j++)
		{
			if(mt.getMinutia(i).distance(mt.getMinutia(j)) <= T) continue;
			for(int k = 0; k < mt.getSize(); k++)
				circle(mat,Point(mt.getMinutia(k).getX()+offsetx,mt.getMinutia(k).getY()+offsety),3,Scalar(0,0,255),CV_FILLED);
			circle(mat,Point(mt.getMinutia(i).getX()+offsetx,mt.getMinutia(i).getY()+offsety),3,Scalar(0,255,0),CV_FILLED);
			circle(mat,Point(mt.getMinutia(j).getX()+offsetx,mt.getMinutia(j).getY()+offsety),3,Scalar(0,255,0),CV_FILLED);
			Minutia m1 = mt.getMinutia(i);
			Minutia m2 = mt.getMinutia(j);

#if 0
			long x1 = m1.getX(), y1 = m1.getY();
			long x2 = m2.getX(), y2 = m2.getY();
			long x0 = mt.getCenterX(), y0 = mt.getCenterY();
			long vx = y1-y2, vy = x2-x1;
			long delta1 = (vx*vx+vy*vy)*R*R+2*vx*vy*(x1-x0)*(y1-y0);
			long delta2 = (vx*vx+vy*vy)*R*R+2*vx*vy*(x2-x0)*(y2-y0);
			long b1 = vx*(x0-x1)+vy*(y0-y1), b2 = vx*(x0-x2)+vy*(y0-y2);
			long a = vx*vx+vy*vy;
			double l1 = fabs(abs(b1)-sqrt(delta1))/a;
			double l2 = fabs(abs(b2)-sqrt(delta2))/a;
			long xx1 = x1 + l1*vx;
			long yy1 = y1 + l1*vy;
			long xx2 = x2 + l2*vx;
			long yy2 = y2 + l2*vy;
			long dx = xx1 - xx2, dy = yy1 - yy2;
			double l = sqrt(dx*dx+dy*dy);
			cout << "l=" << l << "\tphi=" << 2*asin(l/(2*R)) << endl;
#elif 0
			double x1 = m1.getX(), y1 = m1.getY();
			double x2 = m2.getX(), y2 = m2.getY();
			double x0 = mt.getCenterX(), y0 = mt.getCenterY();
			double dx1 = x1-x0, dy1 = y1-y0;
			double dx2 = x2-x0, dy2 = y2-y0;
			// double rr1 = dx1*dx1+dy1*dy1;
			double l = sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
			double d = fabs(dx1*(y2-y1)-dy1*(x2-x1))/l;
			if(d > R) continue;
			double phi = acos(d/R);
			cout << 2 * phi << endl;
#else
			double x1 = m1.getX(), y1 = m1.getY();
			double x2 = m2.getX(), y2 = m2.getY();
			double x0 = mt.getCenterX(), y0 = mt.getCenterY();
			double dx1 = x1-x0, dy1 = y1-y0;
			double dx2 = x2-x0, dy2 = y2-y0;
			double dx = x1-x2, dy = y1-y2;
			double l1 = sqrt(dx1*dx1+dy1*dy1);
			double l2 = sqrt(dx2*dx2+dy2*dy2);
			double ll = sqrt(dx*dx+dy*dy);
			double a1 = acos((dx1*dx+dy1*dy)/(l1*ll));
			double a2 = acos(-(dx2*dx+dy2*dy)/(l2*ll));
			cout << a1 << ' ' << a2 << endl;
#endif

			imshow(name,mat);
			waitKey();
		}
	}
	cout << endl;
}

void drawCodeDiff(const CountCode & code, const CountCode & code2, int num)
{
	int w=800,h=400;
	Mat mat(h,w,CV_8UC3,Scalar(255,255,255));
	int vsize = h/200;
	int hsize = w/code.getSize();
	Point points[MAX_CODE_SIZE];
	for(int i = 0; i < code.getSize(); i++)
	{
		points[i] = Point(i*hsize,h/2 - (code.getAt(i)-code2.getAt(i))*vsize);
		circle(mat,points[i],3,Scalar(0,0,255),CV_FILLED);
	}
	for(int i = 0; i < code.getSize()-1; i++)
		line(mat,points[i],points[i+1],Scalar(0,0,0));
	Point accumPoints[MAX_CODE_SIZE];
	int curr = 0;
	for(int i = 0; i < code.getSize(); i++)
	{
		curr += code.getAt(i)-code2.getAt(i);
		accumPoints[i] = Point(i*hsize,h/2-curr*vsize);
	}
	for(int i = 0; i < code.getSize()-1; i++)
		line(mat,accumPoints[i],accumPoints[i+1],Scalar(255,0,0));
	line(mat,Point(0,h/2),Point(w,h/2),Scalar(0,0,0));
	char buf[10];
	sprintf(buf,"%d",num);
	imshow(buf,mat);
}

void showTemplate(const MinutiaeTemplate & mt, double T, const char *name, const char *filename)
{
	Circle c;
	c.setT(T);
	c.getCenter(mt);
	c.fromTemplate(mt);
	Mat mat(700, 700, CV_8UC3, Scalar(255, 255, 255));
	Mat fingerprint = imread(filename);
	int offsetx = 200, offsety = 0, R = c.getR();
	fingerprint.copyTo(mat.rowRange(offsety,offsety+fingerprint.rows).colRange(offsetx,offsetx+fingerprint.cols));
	circle(mat,Point(c.getCenterX()+offsetx,c.getCenterY()+offsety),R,Scalar(0,255,0));
	circle(mat,Point(c.getCenterX()+offsetx,c.getCenterY()+offsety),3,Scalar(0,255,0),CV_FILLED);
#if 0
	for(int i = 1; i < c.getSize(); i+=2)
	{
		double p1 = c.getPoint(i-1);
		double p2 = c.getPoint(i);
		line(mat,Point(R*cos(p1)+c.getCenterX()+offsetx,R*sin(p1)+c.getCenterY()+offsety),
				Point(R*cos(p2)+c.getCenterX()+offsetx,R*sin(p2)+c.getCenterY()+offsety),
				Scalar(255,0,0));
	}
#endif
	for(int i = 0; i < mt.getSize(); i++)
		circle(mat,Point(mt.getMinutia(i).getX()+offsetx,mt.getMinutia(i).getY()+offsety),3,Scalar(0,0,255),CV_FILLED);
	imshow(name,mat);
}

double computeEER(double rightDistance[][TEMPLATE_NUM], double wrongDistance[][TEMPLATE_NUM], double *t)
{
	double lineDistanceRight[TEST_SET_SIZE*TEMPLATE_NUM];
	double lineDistanceWrong[TEST_SET_SIZE*TEMPLATE_NUM];
	for(int i = 0; i < TEST_SET_SIZE; i++)
	{
		for(int j = 0; j < TEMPLATE_NUM; j++)
		{
			lineDistanceRight[i*TEMPLATE_NUM+j] = rightDistance[i][j];
			lineDistanceWrong[i*TEMPLATE_NUM+j] = wrongDistance[i][j];
		}
	}
	sort(&lineDistanceRight[0],&lineDistanceRight[TEST_SET_SIZE*TEMPLATE_NUM]);
	sort(&lineDistanceWrong[0],&lineDistanceWrong[TEST_SET_SIZE*TEMPLATE_NUM]);
	for(int i = 0; i < TEST_SET_SIZE*TEMPLATE_NUM; i++)
	{
		if(lineDistanceWrong[i] > lineDistanceRight[TEST_SET_SIZE*TEMPLATE_NUM-1-i])
			return *t=lineDistanceWrong[i],(double)i/(TEST_SET_SIZE*TEMPLATE_NUM);
	}
	return 1.0;
}

double getCircleDistance(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double T)
{
	Circle c1,c2;
	c1.setT(T);c2.setT(T);
	CountCode code1(nBin),code2(nBin);
	c1.getCenter(mt1); c2.getCenter(mt2);
	c1.fromTemplate(mt1); c2.fromTemplate(mt2);
	return c1.distance(c2);
}

double getDistDistance(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double T)
{
	Circle c1,c2;
	c1.setT(T);c2.setT(T);
	CountCode code1(nBin),code2(nBin);
	c1.getCenter(mt1); c2.getCenter(mt2);
	c1.getDistanceSetFromTemplate(mt1); c2.getDistanceSetFromTemplate(mt2);
	code1.fromCircle(c1); code2.fromCircle(c2);
	// code1.obfusticate(); code2.obfusticate();
	return code1.distance(code2);
}

double getDistance(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double T)
{
	Circle c1,c2;
	c1.setT(T);c2.setT(T);
	CountCode code1(nBin),code2(nBin);
	c1.getCenter(mt1); c2.getCenter(mt2);
	c1.fromTemplate(mt1); c2.fromTemplate(mt2);
	code1.fromCircle(c1); code2.fromCircle(c2);
	// code1.obfusticate(); code2.obfusticate();
	return code1.distance(code2);
}

double getRotateDistance(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double T)
{
	Circle c1,c2;
	c1.setT(T);c2.setT(T);
	CountCode code1(nBin),code2(nBin);
	c1.getCenter(mt1); c2.getCenter(mt2);
	c1.fromTemplate(mt1); c2.fromTemplate(mt2);
	code1.fromCircle(c1); code2.fromCircle(c2);
	return code1.rotateDistance(code2,nBin/12+1);
}

double getEMDistance(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double T)
{
	Circle c1,c2;
	c1.setT(T);c2.setT(T);
	CountCode code1(nBin),code2(nBin);
	c1.getCenter(mt1); c2.getCenter(mt2);
	c1.fromTemplate(mt1); c2.fromTemplate(mt2);
	code1.fromCircle(c1); code2.fromCircle(c2);
	code1.obfusticate(); code2.obfusticate();
	return code1.emDistance(code2);
}

double getEMRotateDistance(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double T)
{
	Circle c1,c2;
	c1.setT(T);c2.setT(T);
	CountCode code1(nBin),code2(nBin);
	c1.getCenter(mt1); c2.getCenter(mt2);
	c1.fromTemplate(mt1); c2.fromTemplate(mt2);
	code1.fromCircle(c1); code2.fromCircle(c2);
	return code1.emRotateDistance(code2,nBin/12+1);
}

inline bool match(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double t, double T)
{
	return getDistance(mt1,mt2,nBin,T) < t;
}

void randomTest()
{
	MinutiaeTemplate::setMaxXY(MAX_X,MAX_Y);

	MinutiaeTemplate templates[TEMPLATE_NUM];
#ifdef TEST_SET_1
	MinutiaeTemplate testSet1[TEST_SET_SIZE][TEMPLATE_NUM];
	double rightDistance1[TEST_SET_SIZE][TEMPLATE_NUM];
	double wrongDistance1[TEST_SET_SIZE][TEMPLATE_NUM];
#endif
#ifdef TEST_SET_2
	MinutiaeTemplate testSet2[TEST_SET_SIZE][TEMPLATE_NUM];
	double rightDistance2[TEST_SET_SIZE][TEMPLATE_NUM];
	double wrongDistance2[TEST_SET_SIZE][TEMPLATE_NUM];
#endif
#ifdef TEST_SET_3
	MinutiaeTemplate testSet3[TEST_SET_SIZE][TEMPLATE_NUM];
	double rightDistance3[TEST_SET_SIZE][TEMPLATE_NUM];
	double wrongDistance3[TEST_SET_SIZE][TEMPLATE_NUM];
#endif

	cerr << "Generating original templates..." << endl;
	for(int i = 0; i < TEMPLATE_NUM; i++)
		templates[i].fillWithRandom(TEMPLATE_SIZE);
	cerr << "Done generated original templates." << endl;
#ifdef TEST_SET_1
	cerr << "Generating first test set..." << endl;
	for(int i = 0; i < TEST_SET_SIZE; i++)
	{
		for(int j = 0; j < TEMPLATE_NUM; j++)
		{
			testSet1[i][j].copy(templates[i]);
			testSet1[i][j].disturb(disturbScale);
		}
		cerr << "*";
	}
	cerr << endl << "Done generated first test set..." << endl;
#endif
#ifdef TEST_SET_2
	cerr << "Generating second test set..." << endl;
	for(int i = 0; i < TEST_SET_SIZE; i++)
	{
		for(int j = 0; j < TEMPLATE_NUM; j++)
		{
			testSet2[i][j].copy(templates[i]);
			testSet2[i][j].removeOrAddRandom(2);
		}
		cerr << "*";
	}
	cerr << endl << "Done generated second test set..." << endl;
#endif
#ifdef TEST_SET_3
	cerr << "Generating third test set..." << endl;
	for(int i = 0; i < TEST_SET_SIZE; i++)
	{
		for(int j = 0; j < TEMPLATE_NUM; j++)
		{
			testSet3[i][j].copy(templates[i]);
			testSet3[i][j].rotateRandom(M_PI/6);
		}
		cerr << "*";
	}
	cerr << endl << "Done generated third test set..." << endl;
#endif
	double Tset[] = {0,20,40,60,80,100,120};
	int nBinSet[] = {360,180,90,45,30,20};
	double tset[] = {0.5,1.0,1.5,2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0,6.5,7.0,7.5,8.0};
	int nT = sizeof(Tset)/sizeof(Tset[0]);
	int nB = sizeof(nBinSet)/sizeof(nBinSet[0]);
	int nt = sizeof(tset)/sizeof(tset[0]);
	double t = DEFAULT_T2;
	cout << setprecision(10) << endl;
#ifdef TEST_SET_1
	cerr << "Start testing the first test set..." << endl;
	
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
#ifdef COMPUTE_GAR
			cout << T << "," << nBin << ",";
#endif
			time_t start = clock();
			for(int i = 0; i < TEST_SET_SIZE; i++)
			{
				for(int j = 0; j < TEMPLATE_NUM; j++)
				{
#ifdef NO_ROATE_FIRST
					rightDistance1[i][j] = getDistance(templates[i],testSet1[i][j],nBin,T);
					wrongDistance1[i][j] = getDistance(templates[i],testSet1[(i+1)%TEST_SET_SIZE][j],nBin,T);
#else
					rightDistance1[i][j] = getRotateDistance(templates[i],testSet1[i][j],nBin,T);
					wrongDistance1[i][j] = getRotateDistance(templates[i],testSet1[(i+1)%TEST_SET_SIZE][j],nBin,T);
#endif
				}
			}
			cout << (double)(clock() - start)/(TEST_SET_SIZE*TEMPLATE_NUM*1000) << ",";
#ifdef OUTPUT_DISTANCE
			for(int i = 0; i < TEST_SET_SIZE; i++)
				for(int j = 0; j < TEMPLATE_NUM; j++)
					cout << rightDistance1[i][j] << " ";
			cout << endl;
			for(int i = 0; i < TEST_SET_SIZE; i++)
				for(int j = 0; j < TEMPLATE_NUM; j++)
					cout << wrongDistance1[i][j] << " ";
			cout << endl;
#endif
#ifdef COMPUTE_GAR
#if 0
			for(int it = 0; it < nt; it++)
			{
				int correct = 0;
				int fwrong = 0;
				double t = tset[it];
				for(int i = 0; i < TEST_SET_SIZE; i++)
				{
					for(int j = 0; j < TEMPLATE_NUM; j++)
					{
						if(rightDistance1[i][j] < t)
							correct++;
						if(wrongDistance1[i][j] < t)
							fwrong++;
					}
				}
				cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << ",";
				cout << (double)fwrong/(TEST_SET_SIZE*TEMPLATE_NUM);
				if(it < nt-1) cout << ",";
			}
			cout << endl;
#endif
			double t;
			cout << computeEER(rightDistance1,wrongDistance1,&t) << ',' << t << endl;
#endif
		}
	}
#endif
#ifdef TEST_SET_2
	cerr << "Start testing the second test set..." << endl;
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
#ifdef COMPUTE_GAR
			cout << T << "," << nBin << ",";
#endif
			time_t start = clock();
			for(int i = 0; i < TEST_SET_SIZE; i++)
			{
				for(int j = 0; j < TEMPLATE_NUM; j++)
				{
#ifdef NO_ROTATE_SECOND
					rightDistance2[i][j] = getDistance(templates[i],testSet2[i][j],nBin,T);
					wrongDistance2[i][j] = getDistance(templates[i],testSet2[(i+1)%TEST_SET_SIZE][j],nBin,T);
#else
					rightDistance2[i][j] = getRotateDistance(templates[i],testSet2[i][j],nBin,T);
					wrongDistance2[i][j] = getRotateDistance(templates[i],testSet2[(i+1)%TEST_SET_SIZE][j],nBin,T);
#endif
				}
			}
			cout << (double)(clock() - start)/(TEST_SET_SIZE*TEMPLATE_NUM*1000) << ",";
#ifdef OUTPUT_DISTANCE
			for(int i = 0; i < TEST_SET_SIZE; i++)
				for(int j = 0; j < TEMPLATE_NUM; j++)
					cout << rightDistance2[i][j] << " ";
			cout << endl;
			for(int i = 0; i < TEST_SET_SIZE; i++)
				for(int j = 0; j < TEMPLATE_NUM; j++)
					cout << wrongDistance2[i][j] << " ";
			cout << endl;
#endif
#ifdef COMPUTE_GAR
#if 0
			for(int it = 0; it < nt; it++)
			{
				int correct = 0;
				int fwrong = 0;
				double t = tset[it];
				for(int i = 0; i < TEST_SET_SIZE; i++)
				{
					for(int j = 0; j < TEMPLATE_NUM; j++)
					{
						if(rightDistance2[i][j] < t)
							correct++;
						if(wrongDistance2[i][j] < t)
							fwrong++;
					}
				}
				cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << ",";
				cout << (double)fwrong/(TEST_SET_SIZE*TEMPLATE_NUM);
				if(it < nt-1) cout << ",";
			}
			cout << endl;
#endif
			double t;
			cout << computeEER(rightDistance2,wrongDistance2,&t) << ',' << t << endl;
#endif
		}
	}
#endif
#ifdef TEST_SET_3
	cerr << "Start testing the third test set..." << endl;
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
#ifdef COMPUTE_GAR
			cout << T << "," << nBin << ",";
#endif
			time_t start = clock();
			for(int i = 0; i < TEST_SET_SIZE; i++)
			{
				for(int j = 0; j < TEMPLATE_NUM; j++)
				{
#ifdef NO_ROTATE_THIRD
					rightDistance3[i][j] = getDistance(templates[i],testSet3[i][j],nBin,T);
					wrongDistance3[i][j] = getDistance(templates[i],testSet3[(i+1)%TEST_SET_SIZE][j],nBin,T);
#else
					rightDistance3[i][j] = getRotateDistance(templates[i],testSet3[i][j],nBin,T);
					wrongDistance3[i][j] = getRotateDistance(templates[i],testSet3[(i+1)%TEST_SET_SIZE][j],nBin,T);
#endif
				}
			}
			cout << (double)(clock() - start)/(TEST_SET_SIZE*TEMPLATE_NUM*1000) << ",";
#ifdef OUTPUT_DISTANCE
			for(int i = 0; i < TEST_SET_SIZE; i++)
				for(int j = 0; j < TEMPLATE_NUM; j++)
					cout << rightDistance3[i][j] << " ";
			cout << endl;
			for(int i = 0; i < TEST_SET_SIZE; i++)
				for(int j = 0; j < TEMPLATE_NUM; j++)
					cout << wrongDistance3[i][j] << " ";
			cout << endl;
#endif
#ifdef COMPUTE_GAR
#if 0
			for(int it = 0; it < nt; it++)
			{
				int correct = 0;
				int fwrong = 0;
				double t = tset[it];
				for(int i = 0; i < TEST_SET_SIZE; i++)
				{
					for(int j = 0; j < TEMPLATE_NUM; j++)
					{
						if(rightDistance3[i][j] < t)
							correct++;
						if(wrongDistance3[i][j] < t)
							fwrong++;
					}
				}
				cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << ",";
				cout << (double)fwrong/(TEST_SET_SIZE*TEMPLATE_NUM);
				if(it < nt-1) cout << ",";
			}
			cout << endl;
#endif
			double t;
			cout << computeEER(rightDistance3,wrongDistance3,&t) << ',' << t << endl;
#endif
		}
	}
#endif
}

void testWithFingerprints()
{
	MinutiaeTemplate templates[FINGERPRINT_NUM][FINGERPRINT_SET_SIZE];
	Circle circles[FINGERPRINT_NUM][FINGERPRINT_SET_SIZE];
	CountCode countCodes[FINGERPRINT_NUM][FINGERPRINT_SET_SIZE];
	double rightDistance[FINGERPRINT_NUM][FINGERPRINT_SET_SIZE];
	double wrongDistance[FINGERPRINT_NUM][FINGERPRINT_SET_SIZE];
	cerr << "Reading templates from fingerprint images..." << endl;
	for(int i = 0; i < FINGERPRINT_NUM; i++)
	{
		for(int j = 0; j < FINGERPRINT_SET_SIZE; j++)
		{
			// cerr << "Reading " << getFullPath(filename,i+1,j+1) << endl;
			FingerprintReader fpr(getFullPath(filenames[i][j],i+1,j+1));
			templates[i][j].getFromFingerprintReader(fpr,TEMPLATE_SIZE);
			templates[i][j].setFilename(filenames[i][j]);
			templates[i][j].setReferencePoint(
					referencePointX[i*2+j],referencePointY[i*2+j],
					0);
					//referencePointA[i*2+j]);
		}
	}
	cerr << "Done reading templates from fingerprint images..." << endl;

	double Tset[] = {0,20,40,60,80,100,120};
	int nBinSet[] = {360,180,90,45,30,20};
	int nT = sizeof(Tset)/sizeof(Tset[0]);
	int nB = sizeof(nBinSet)/sizeof(nBinSet[0]);

#ifdef CALCULATE_DISTANCE
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];

			for(int i = 0; i < FINGERPRINT_NUM; i++)
			{
				for(int j = 0; j < FINGERPRINT_SET_SIZE; j++)
				{
					rightDistance[i][j] = getDistDistance(templates[i][0],templates[i][j],nBin,T);
					wrongDistance[i][j] = getDistDistance(templates[i][0],templates[(i+1)%FINGERPRINT_NUM][j],nBin,T);
				}
			}
			for(int i = 0; i < FINGERPRINT_NUM; i++)
			{
				for(int j = 1; j < FINGERPRINT_SET_SIZE; j++)
				{
					cout << rightDistance[i][j] << " ";
				}
			}
			cout << endl;
			for(int i = 0; i < FINGERPRINT_NUM; i++)
			{
				for(int j = 1; j < FINGERPRINT_SET_SIZE; j++)
				{
					cout << wrongDistance[i][j] << " ";
				}
			}
			cout << endl;
		}
	}
#elif 0
	for(int i = 0; i < FINGERPRINT_NUM; i++)
	{
		for(int j = 0; j < FINGERPRINT_SET_SIZE; j++)
		{
			int iT = 2;
			int iB = 2;
			double T = Tset[iT];
			int nBin = nBinSet[iB];
			circles[i][j].setT(T);
			circles[i][j].getCenter(templates[i][j]);
			CountCode code(nBin);
			// circles[i][j].fromTemplate(templates[i][j]);
			circles[i][j].getDistanceSetFromTemplate(templates[i][j]);
			code.fromCircle(circles[i][j]);
			countCodes[i][j] = code;
		}
	}
	for(int i = 0; i < FINGERPRINT_NUM-1; i++)
	{
		// double d1 = countCodes[i][0].emDistance(countCodes[i][1]);
		// double d2 = countCodes[i][0].emDistance(countCodes[i+1][1]);
		// cout << d1 << ' ' << d2 << endl;
		// cout << countCodes[i][0].getSum() << ' ' << countCodes[i][1].getSum() << endl;
		// cout << templates[i][0].getSize() << ' ' << templates[i][1].getSize() << endl;
		// drawCodeDiff(countCodes[i][0],countCodes[i][1],0);
		// drawCodeDiff(countCodes[i][0],countCodes[i+1][1],1);
		//
		// drawCircle(circles[i][0],circles[i][1],0);
		// drawCircle(circles[i][0],circles[i+1][0],1);
		// showTemplate(templates[i][0], 0, "img0", filenames[i][0]);
		// showTemplate(templates[i][1], 0, "img1", filenames[i][1]);
		// showTemplate(templates[i+1][0], 0, "img2", filenames[i+1][0]);
		cout << circles[i][0].distance(circles[i][1]) << ' ' << circles[i][0].distance(circles[i+1][0]) << endl;
		// waitKey();
	}
#else
	for(int i = 0; i < FINGERPRINT_NUM; i++)
	{
		showProcess(templates[i][0],0,"0",filenames[i][0]);
		showProcess(templates[i][1],0,"1",filenames[i][1]);
	}
#endif
}

int main()
{
	srand((unsigned)time(0));
#if 0
	randomTest();
#else
	testWithFingerprints();
#endif
	return 0;
}
