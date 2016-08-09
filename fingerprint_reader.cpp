#include <iostream>
#include <iomanip>
#include <fstream>
#include "libfjfx.h"
#include "getdata.hpp"


class FingerprintReader {
};

int getData(char tmpl[], const char *filename)
{
	ifstream fin;
	int height, width, gray;
	char tmp;
	unsigned int size;
	void * image = NULL;
	int err;

	fin.open(filename, ios::binary);
	if (!fin) {
		cerr << "Cannot open image file: " << filename << endl;
		return -1;
	}
	fin >> tmp >> tmp;
	fin >> width >> height >> gray;
	if (gray > 256 || width > 0xffff || height > 0xffff ||
		gray <= 1 || width < 32 || height < 32) {
		cerr << "Image file " << filename << " is in unsupported format " << endl;
		fin.close();
		return -1;
	}

	size = width * height;
	image = new char[size];
	if (image == 0) {
		cerr << "Cannot allocate image buffer: image size is  " << width << "x" << height;
		return -1;
	}

	fin.read((char*)image, size);
	fin.close();

	size = FJFX_FMD_BUFFER_SIZE;
	err = fjfx_create_fmd_from_raw(image, 500, height, width, FJFX_FMD_ISO_19794_2_2005, tmpl, &size);
	delete image; image = 0;
	if (err != FJFX_SUCCESS) {
		printf("Failed feature extraction\n");
		return -1;
	}

	return size;
}
