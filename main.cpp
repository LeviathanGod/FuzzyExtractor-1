#include <iostream>
#include <cmath>
//#include <opencv2/opencv.hpp>
#include "minutiae_template.h"
#include "circle.h"
#include "code.h"

#define TEST_SET_1
#define TEST_SET_2
#define TEST_SET_3

const int TEMPLATE_SIZE = 20;
const int MAX_X = 255;
const int MAX_Y = 255;
const int TEMPLATE_NUM = 100;
const int TEST_SET_SIZE = 20;
const int disturbScale = 5;
const double DEFAULT_T2 = 5.0;

using namespace std;
//using namespace cv;

double getDistance(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double T)
{
	Circle c1,c2;
	c1.setT(T);c2.setT(T);
	CountCode code1(nBin),code2(nBin);
	c1.getCenter(mt1); c2.getCenter(mt2);
	c1.fromTemplate(mt1); c2.fromTemplate(mt2);
	code1.fromCircle(c1); code2.fromCircle(c2);
	return code1.distance(code2);
}

inline bool match(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double t, double T)
{
	return getDistance(mt1,mt2,nBin,T) < t;
}

int main()
{
	srand((unsigned)time(0));
	MinutiaeTemplate::setMaxXY(MAX_X,MAX_Y);

	MinutiaeTemplate templates[TEMPLATE_NUM];
#ifdef TEST_SET_1
	MinutiaeTemplate testSet1[TEST_SET_SIZE][TEMPLATE_NUM];
#endif
#ifdef TEST_SET_2
	MinutiaeTemplate testSet2[TEST_SET_SIZE][TEMPLATE_NUM];
#endif
#ifdef TEST_SET_3
	MinutiaeTemplate testSet3[TEST_SET_SIZE][TEMPLATE_NUM];
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
	double tset[] = {2.0,2.5,3.0,3.5,4.0,4.5,5.0,5.5,6.0};
	int nT = sizeof(Tset)/sizeof(Tset[0]);
	int nB = sizeof(nBinSet)/sizeof(nBinSet[0]);
	int nt = sizeof(tset)/sizeof(tset[0]);
	double t = DEFAULT_T2;
#ifdef TEST_SET_1
	cerr << "Start testing the first test set..." << endl;
	cout << "First test set" << endl;
	
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
			cout << T << "," << nBin << ",";
			for(int it = 0; it < nt; it++)
			{
				double t = tset[it];
				int correct = 0;
				int fwrong = 0;
				// double match_distance = 0;
				// double wrong_distance = 0;
				for(int i = 0; i < TEST_SET_SIZE; i++)
				{
					for(int j = 0; j < TEMPLATE_NUM; j++)
					{
						// match_distance += getDistance(templates[i],testSet1[i][j],nBin,T);
						// wrong_distance += getDistance(templates[i],testSet1[(i+1)%TEST_SET_SIZE][j],nBin,T);
						if(match(templates[i],testSet1[i][j],nBin,t,T))
							correct++;
						if(match(templates[i],testSet1[(i+1)%TEST_SET_SIZE][j],nBin,t,T))
							fwrong++;
					}
				}
				cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << ",";
				cout << (double)fwrong/(TEST_SET_SIZE*TEMPLATE_NUM);
				if(it < nt-1) cout << ",";
				// cout << "Average match distance: " << match_distance/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
				// cout << "Average wrong distance: " << wrong_distance/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
			}
			cout << endl;
		}
	}
#endif
#ifdef TEST_SET_2
	cerr << "Start testing the second test set..." << endl;
	cout << "Second test set" << endl;
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
			cout << T << "," << nBin << ",";
			for(int it = 0; it < nt; it++)
			{
				double t = tset[it];
				int correct = 0;
				int fwrong = 0;
				// double match_distance = 0;
				// double wrong_distance = 0;
				for(int i = 0; i < TEST_SET_SIZE; i++)
				{
					for(int j = 0; j < TEMPLATE_NUM; j++)
					{
						// match_distance += getDistance(templates[i],testSet2[i][j],nBin,T);
						// wrong_distance += getDistance(templates[i],testSet2[(i+1)%TEST_SET_SIZE][j],nBin,T);
						if(match(templates[i],testSet2[i][j],nBin,t,T))
							correct++;
						if(match(templates[i],testSet2[(i+1)%TEST_SET_SIZE][j],nBin,t,T))
							fwrong++;
					}
				}
				cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << ",";
				cout << (double)fwrong/(TEST_SET_SIZE*TEMPLATE_NUM);
				if(it < nt-1) cout << ",";
				// cout << "Average match distance: " << match_distance/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
				// cout << "Average wrong distance: " << wrong_distance/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
			}
			cout << endl;
		}
	}
#endif
#ifdef TEST_SET_3
	cerr << "Start testing the third test set..." << endl;
	cout << "Third test set" << endl;
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
			cout << T << "," << nBin << ",";
			for(int it = 0; it < nt; it++)
			{
				double t = tset[it];
				int correct = 0;
				int fwrong = 0;
				// double match_distance = 0;
				// double wrong_distance = 0;
				for(int i = 0; i < TEST_SET_SIZE; i++)
				{
					for(int j = 0; j < TEMPLATE_NUM; j++)
					{
						// match_distance += getDistance(templates[i],testSet3[i][j],nBin,T);
						// wrong_distance += getDistance(templates[i],testSet3[(i+1)%TEST_SET_SIZE][j],nBin,T);
						if(match(templates[i],testSet3[i][j],nBin,it,T))
							correct++;
						if(match(templates[i],testSet3[(i+1)%TEST_SET_SIZE][j],nBin,t,T))
							fwrong++;
					}
				}
				cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << ",";
				cout << (double)fwrong/(TEST_SET_SIZE*TEMPLATE_NUM);
				if(it < nt-1) cout << ",";
				// cout << "Average match distance: " << match_distance/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
				// cout << "Average wrong distance: " << wrong_distance/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
			}
			cout << endl;
		}
	}
#endif
	return 0;
}
