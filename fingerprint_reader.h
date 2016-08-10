#ifndef __FINGERPRINT_READER_H__
#define __FINGERPRINT_READER_H__

const int FEATURE_BUF_SIZE = 60;

typedef struct {
	unsigned int format;
	unsigned int version;
	unsigned int length;
	unsigned short dev_info;
	unsigned short width;
	unsigned short height;
	unsigned short res_x;
	unsigned short res_y;
	unsigned char finger_num;
	unsigned char zero;
	unsigned char finger_pos;
	unsigned char view_num;
	unsigned char quality;
	unsigned char minutae_num;
} header;

typedef struct {
	unsigned short X;
	unsigned short Y;
	unsigned char theta;
	unsigned char quality;
} feature_;

typedef struct {
	unsigned char type;
	unsigned short X;
	unsigned short Y;
	unsigned char theta;
	unsigned char quality;
} feature;


inline unsigned short exchange(unsigned short X)
{
	return ((X & 0xFF) << 8) + ((X & 0xFF00) >> 8);
}

int getData(char output[], const char *filename);
int readData(feature output[], const char *filename);

class FingerprintReader {
	int size;
	feature features[FEATURE_BUF_SIZE];
public:
	FingerprintReader():size(0){}
	FingerprintReader(const char * filename):size(0){open(filename);}
	void open(const char * filename);
	inline int getSize()const{return size;}
	inline feature getFeature(int i)const{return features[i];}
	inline int getFeatureX(int i)const{return features[i].X;}
	inline int getFeatureY(int i)const{return features[i].Y;}
};

#endif
