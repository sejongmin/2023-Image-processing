#define _CRT_SECURE_NO_WARNINGS 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>

int main() {
	BITMAPFILEHEADER bmpFile, bmpFile1;
	BITMAPINFOHEADER bmpInfo, bmpInfo1;
	FILE* inputFile = NULL, *inputFile1 = NULL;

	inputFile = fopen("reconEncY.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	inputFile1 = fopen("reconDecY.bmp", "rb");
	fread(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, inputFile1);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

	unsigned char* inputImg = NULL, * inputImg1 = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);

	unsigned char* Y = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y[j * width + i] = inputImg[j * stride + 3 * i];
			Y1[j * width + i] = inputImg1[j * stride + 3 * i];
		}
	}

	double mse = 0, psnr;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			mse += (double)((Y1[j * width + i] - Y[j * width + i]) * (Y1[j * width + i] - Y[j * width + i]));
		}
	}
	mse /= (height * width);
	psnr = mse != 0 ? 10 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.2f\nPSNR = %.2f dB", mse, psnr);

	return 0;
}