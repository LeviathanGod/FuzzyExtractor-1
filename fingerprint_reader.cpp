#include <iostream>
#include <iomanip>
#include <fstream>
#include "fjfx.h"
#include "fingerprint_reader.h"

void FingerprintReader::open(const char * filename)
{
	size = readData(features,filename);
}

int readData(feature output[], const char *filename)
{
	char tmpl[FJFX_FMD_BUFFER_SIZE];
	int size = getData(tmpl, filename);
	if (size <= 0)
	{
		std::cerr << "Failed to read data from " << filename << std::endl;
		return -1;
	}
	header * pheader = (header *)tmpl;
	int num = pheader->minutae_num;

	feature_ * pbuf = (feature_*)(tmpl + sizeof(header));
	int i;
	for (i = 0; i < num && i < FEATURE_BUF_SIZE; i++)
	{
		output[i].type = exchange(pbuf[i].X) >> 14;
		output[i].X = exchange(pbuf[i].X) & 0x3FFF;
		output[i].Y = exchange(pbuf[i].Y);
		output[i].theta = pbuf[i].theta;
		output[i].quality = pbuf[i].quality;
	}

	return i;
}

int getData(char tmpl[], const char *filename)
{
	std::ifstream fin;
	int height, width, gray;
	char tmp;
	unsigned int size;
	char * image = NULL;
	int err;

	fin.open(filename, std::ios::binary);
	if (!fin) {
		std::cerr << "Cannot open image file: " << filename << std::endl;
		return -1;
	}
	fin >> tmp >> tmp;
	fin >> width >> height >> gray;
	if (gray > 256 || width > 0xffff || height > 0xffff ||
		gray <= 1 || width < 32 || height < 32) {
		std::cerr << "Image file " << filename << " is in unsupported format " << std::endl;
		fin.close();
		return -1;
	}

	size = width * height;
	image = new char[size];
	if (image == 0) {
		std::cerr << "Cannot allocate image buffer: image size is  " << width << "x" << height;
		return -1;
	}

	fin.read((char*)image, size);
	fin.close();

	size = FJFX_FMD_BUFFER_SIZE;
	err = fjfx_create_fmd_from_raw(image, 500, height, width, FJFX_FMD_ISO_19794_2_2005, tmpl, &size);
	delete image; image = 0;
	if (err != FJFX_SUCCESS) {
		std::cerr << "Failed feature extraction" << std::endl;
		return -1;
	}

	return size;
}
