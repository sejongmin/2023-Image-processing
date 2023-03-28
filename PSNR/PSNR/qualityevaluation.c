#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;

	inputFile = fopen("AICenterY_Noise.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

	BITMAPFILEHEADER bmpFile1;
	BITMAPINFOHEADER bmpInfo1;
	FILE* inputFile1 = NULL;

	inputFile1 = fopen("AICenterY.bmp", "rb");
	fread(&bmpFile1, sizeof(BITMAPFILEHEADER), 1, inputFile1);
	fread(&bmpInfo1, sizeof(BITMAPINFOHEADER), 1, inputFile1);

	BITMAPFILEHEADER bmpFile2;
	BITMAPINFOHEADER bmpInfo2;
	FILE* inputFile2 = NULL;

	inputFile2 = fopen("AICenter.bmp", "rb");
	fread(&bmpFile2, sizeof(BITMAPFILEHEADER), 1, inputFile2);
	fread(&bmpInfo2, sizeof(BITMAPINFOHEADER), 1, inputFile2);

	unsigned char* inputImg = NULL;
	unsigned char* inputImg1 = NULL;
	unsigned char* inputImg2 = NULL;
	unsigned char* inputImg2_ = NULL;


	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	

	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);
	fread(inputImg2, sizeof(unsigned char), size, inputFile2);

	double Y, Cb, Cr, R, G, B;
	double mse1 = 0, mse2 = 0, psnr1, psnr2;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			inputImg[width * j + i] = inputImg[j * stride + 3 * i + 0];	//Noise
			inputImg1[width * j + i] = inputImg1[j * stride + 3 * i + 0];
			mse1 += (double)((inputImg1[width * j + i] - inputImg[width * j + i]) * (inputImg1[width * j + i] - inputImg[width * j + i]));

			Y = 0.299 * inputImg2[j * stride + 3 * i + 2] + 0.587 * inputImg2[j * stride + 3 * i + 1] + 0.114 * inputImg2[j * stride + 3 * i + 0];
			inputImg2[j * width + i] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));

			mse2 += (double)((inputImg2[width * j + i] - inputImg[width * j + i]) * (inputImg2[width * j + i] - inputImg[width * j + i]));

		}
	}
	mse1 /= (width * height);
	psnr1 = mse1 != 0.0 ? 10.0 * log10(255 * 255 / mse1) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse1, psnr1);

	mse2 /= (width * height);
	psnr2 = mse2 != 0.0 ? 10.0 * log10(255 * 255 / mse2) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse2, psnr2);

	return 0;
}