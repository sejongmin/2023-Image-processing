#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;

	inputFile = fopen("AICenterY_Subsampling.bmp", "rb");
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
	int width1 = bmpInfo1.biWidth;
	int height1= bmpInfo1.biHeight;
	int size1 = bmpInfo1.biSizeImage;
	int bitCnt1 = bmpInfo1.biBitCount;
	int stride1 = (((bitCnt1 / 8) * width1) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width1, stride1, height1, size1, bitCnt1);



	int ratio = 2;
	int width2 = bmpInfo.biWidth << ratio;
	int height2 = bmpInfo.biHeight << ratio;
	int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
	int size2 = stride2 * height2;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width2, stride2, height2, size2, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL, * inputImg1 = NULL ;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg1 = (unsigned char*)calloc(size1, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg1, sizeof(unsigned char), size1, inputFile1);

	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width2 * height2, sizeof(unsigned char));

	double mse = 0, psnr;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
		}
	}

	for (int j = 0; j < height2; j++) {
		for (int i = 0; i < width2; i++) {
			Y2[j * width2 + i] = Y1[(j >> ratio) * width + (i >> ratio)];
		}
	}

	for (int j = 0; j < height2; j++) {
		for (int i = 0; i < width2; i++) {
			outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];

		}
	}
	for (int j = 0; j < height1; j++) {
		for (int i = 0; i < width1; i++) {
			mse += (double)((inputImg1[stride * j + i * 3] - outputImg[stride * j + i * 3]) * (inputImg1[stride * j + i * 3] - outputImg[stride * j + i * 3]));
		}
	}
	mse /= (width1 * height1);
	psnr = mse != 0.0 ? 10.0 * log10(255 * 255 / mse) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse, psnr);


	FILE* outputFile = fopen("AICenterY_Upsampling.bmp", "wb");
	bmpInfo.biWidth = width2;
	bmpInfo.biHeight = height2;
	bmpInfo.biSizeImage = size2;
	bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size2;
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size2, outputFile);

	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}