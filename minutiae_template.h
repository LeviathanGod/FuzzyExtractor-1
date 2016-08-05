#ifndef __MINUTIAE_TEMPLATE_H__
#define __MINUTIAE_TEMPLATE_H__

#include <cstdlib>

const unsigned int MINUTIAE_BUF_SIZE = 60;
const unsigned int DEFAULT_MAX_X = 400;
const unsigned int DEFAULT_MAX_Y = 400;

class Minutia {
	int x,y;
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
public:
	MinutiaeTemplate():size(0){}
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
};

#endif
