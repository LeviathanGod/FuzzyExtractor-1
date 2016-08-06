#include <iostream>
#include <cmath>
//#include <opencv2/opencv.hpp>
#include "minutiae_template.h"
#include "circle.h"
#include "code.h"

const int TEMPLATE_SIZE = 20;
const int MAX_X = 255;
const int MAX_Y = 255;
const int TEMPLATE_NUM = 10;
const int TEST_SET_SIZE = 5;
const int disturbScale = 5;

using namespace std;
//using namespace cv;

bool match(const MinutiaeTemplate & mt1, const MinutiaeTemplate & mt2, int nBin, double t, double T)
{
	Circle c1,c2;
	c1.setT(T);c2.setT(T);
	CountCode code1(nBin),code2(nBin);
	c1.getCenter(mt1); c2.getCenter(mt2);
	c1.fromTemplate(mt1); c2.fromTemplate(mt2);
	code1.fromCircle(c1); code2.fromCircle(c2);
	return code1.distance(code2) < t;
}

int main()
{
	srand((unsigned)time(0));
	MinutiaeTemplate::setMaxXY(MAX_X,MAX_Y);

	MinutiaeTemplate templates[TEMPLATE_NUM];
	MinutiaeTemplate testSet1[TEST_SET_SIZE][TEMPLATE_NUM];
	MinutiaeTemplate testSet2[TEST_SET_SIZE][TEMPLATE_NUM];
	MinutiaeTemplate testSet3[TEST_SET_SIZE][TEMPLATE_NUM];
	cout << "Generating original templates..." << endl;
	for(int i = 0; i < TEMPLATE_NUM; i++)
		templates[i].fillWithRandom(TEMPLATE_SIZE);
	cout << "Done generated original templates." << endl;
	cout << "Generating first test set..." << endl;
	for(int i = 0; i < TEST_SET_SIZE; i++)
	{
		for(int j = 0; j < TEMPLATE_NUM; i++)
		{
			testSet1[i][j].copy(templates[j]);
			testSet1[i][j].disturb(disturbScale);
		}
		cout << "*";
	}
	cout << endl << "Done generated first test set..." << endl;
	cout << "Generating second test set..." << endl;
	for(int i = 0; i < TEST_SET_SIZE; i++)
	{
		for(int j = 0; j < TEMPLATE_NUM; i++)
		{
			testSet2[i][j].copy(templates[j]);
			testSet2[i][j].removeOrAddRandom(2);
		}
		cout << "*";
	}
	cout << endl << "Done generated second test set..." << endl;
	cout << "Generating third test set..." << endl;
	for(int i = 0; i < TEST_SET_SIZE; i++)
	{
		for(int j = 0; j < TEMPLATE_NUM; i++)
		{
			testSet3[i][j].copy(templates[j]);
			testSet3[i][j].rotateRandom(M_PI/6);
		}
		cout << "*";
	}
	cout << endl << "Done generated third test set..." << endl;
	double Tset[] = {0,20,40,60,80,100,120};
	int nBinSet[] = {360,180,90,45,30,20};
	int nT = sizeof(Tset)/sizeof(Tset[0]);
	int nB = sizeof(nBinSet)/sizeof(nBinSet[0]);
	double t = 2.0;
	cout << "Start testing the first test set..." << endl;
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
			cout << T << "," << nBin << ",";
			int correct = 0;
			for(int i = 0; i < TEST_SET_SIZE; i++)
			{
				for(int j = 0; j < TEMPLATE_NUM; j++)
				{
					if(match(templates[i],testSet1[i][j],nBin,t,T))
						correct++;
				}
			}
			cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
		}
	}
	cout << "Start testing the second test set..." << endl;
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
			cout << T << "," << nBin << ",";
			int correct = 0;
			for(int i = 0; i < TEST_SET_SIZE; i++)
			{
				for(int j = 0; j < TEMPLATE_NUM; j++)
				{
					if(match(templates[i],testSet2[i][j],nBin,t,T))
						correct++;
				}
			}
			cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
		}
	}
	cout << "Start testing the third test set..." << endl;
	for(int iT = 0; iT < nT; iT++)
	{
		double T = Tset[iT];
		for(int iB = 0; iB < nB; iB++)
		{
			int nBin = nBinSet[iB];
			cout << T << "," << nBin << ",";
			int correct = 0;
			for(int i = 0; i < TEST_SET_SIZE; i++)
			{
				for(int j = 0; j < TEMPLATE_NUM; j++)
				{
					if(match(templates[i],testSet3[i][j],nBin,t,T))
						correct++;
				}
			}
			cout << (double)correct/(TEST_SET_SIZE*TEMPLATE_NUM) << endl;
		}
	}
	return 0;
}
