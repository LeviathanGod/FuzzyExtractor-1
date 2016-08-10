#ifndef __MINUTIAE_TEMPLATE_H__
#define __MINUTIAE_TEMPLATE_H__

#include <cstdlib>
#include "fingerprint_reader.h"

const unsigned int MINUTIAE_BUF_SIZE = 30;
const unsigned int DEFAULT_MAX_X = 400;
const unsigned int DEFAULT_MAX_Y = 400;

class Minutia {
	short x,y;
public:
	Minutia():x(-1),y(-1){}
	Minutia(int _x,int _y):x(_x),y(_y){}
	inline int getX() const {return x;}
	inline int getY() const {return y;}
	inline void setX(int _x) {x=_x;}
	inline void setY(int _y) {y=_y;}
};

double distance(const Minutia & m1, const Minutia & m2);

class MinutiaeTemplate {
	int size;
	static unsigned int maxx;
	static unsigned int maxy;
	Minutia minutiae[MINUTIAE_BUF_SIZE];
	const char * filename = NULL;
	int centerx, centery;
public:
	MinutiaeTemplate():size(0){}
	inline const char * getFilename()const{return filename;}
	inline void setFilename(const char* file){filename=file;}
	void setReferencePoint();
	inline int getCenterX()const{return centerx;}
	inline int getCenterY()const{return centery;}
	inline int getSize() const {return size;} 
	inline static void setMaxX(unsigned int _maxx){maxx=_maxx;}
	inline static void setMaxY(unsigned int _maxy){maxy=_maxy;}
	inline static void setMaxXY(unsigned int _maxx,unsigned int _maxy){maxx=_maxx,maxy=_maxy;}
	inline Minutia getMinutia(int i) const {return minutiae[i];}
	inline bool add(int x,int y) {
		if(size >= MINUTIAE_BUF_SIZE)
			return false;
		minutiae[size++] = Minutia(x,y);
		return true;
	}
	inline bool addRandom(){
		return add(rand()%maxx,rand()%maxy);
	}
	inline bool fillWithRandom(int number) {
		if(number > MINUTIAE_BUF_SIZE) return false;
		size = 0;
		while(size < number)
			addRandom();
		return true;
	}
	inline void copy(const MinutiaeTemplate & rh)
	{
		size = rh.size;
		for(int i = 0; i < size; i++)
			minutiae[i] = rh.minutiae[i];
	}
	inline void disturb(int scale)
	{
		for(int i = 0; i < size; i++)
		{
			minutiae[i].setX(minutiae[i].getX()+rand()%(scale*2+1)-scale);
			minutiae[i].setY(minutiae[i].getY()+rand()%(scale*2+1)-scale);
		}
	}
	inline void remove(int index)
	{
		if(index >= 0 && index < size)
			minutiae[index] = minutiae[--size];
	}
	inline void removeRandom()
	{
		if(size > 0) remove(rand()%size);
	}
	inline void removeRandoms(int k)
	{
		for(int i = 0; i < k && size > 0; i++)
			remove(rand()%size);
	}
	inline void removeOrAddRandom(int k)
	{
		int K = rand()%(2*k+1)-k;
		if(K > 0)
		{
			while(size < MINUTIAE_BUF_SIZE && K-->0)
				addRandom();
		}
		else if(K < 0)
			removeRandoms(-K);
	}
	void rotate(double degree);
	inline void rotateRandom(double degree)
	{
		rotate(rand()*(2*degree)/RAND_MAX-degree);
	}
	inline void getFromFingerprintReader(const FingerprintReader & fpr, int num)
	{
		for(int i = 0; i < fpr.getSize() && i < num; i++)
		{
			int x = fpr.getFeatureX(i);
			int y = fpr.getFeatureY(i);
			add(x,y);
		}
	}
};

#endif
