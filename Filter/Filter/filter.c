#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;

	inputFile = fopen("AICenterY_CombinedNoise.bmp", "rb");
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

	unsigned char* inputImg = NULL, *inputImg1 = NULL, *outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg1, sizeof(unsigned char), size, inputFile1);
	fread(outputImg, sizeof(unsigned char), size, inputFile);

	double Filter[3][3] = { {1.0 / 9, 1.0 / 9, 1.0 / 9}, {1.0 / 9, 1.0 / 9, 1.0 / 9} , {1.0 / 9, 1.0 / 9, 1.0 / 9} };
	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					Y1[j * width + i] += Filter[y][x] * inputImg[(j - y - 1) * stride + 3 * (i - x - 1) + 0];
				}
			}
			outputImg[j * stride + 3 * i + 0] = Y1[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y1[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y1[j * width + i];
		}
	}

	FILE* outputFile = NULL;
	outputFile = fopen("AICenterY_noise.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(inputImg, sizeof(unsigned char), size, outputFile);

	double mse1 = 0, mse2 = 0, psnr;

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			inputImg[j * width + i] = inputImg[j * stride + i * 3 + 0];
			inputImg1[j * width + i] = inputImg1[j * stride + i * 3 + 0];
			outputImg[j * width + i] = outputImg[j * stride + i * 3 + 0];
			mse1 += (inputImg1[j * width + i] - outputImg[j * width + i]) * (inputImg1[j * width + i] - outputImg[j * width + i]);
			mse2 += (inputImg1[j * width + i] - inputImg[j * width + i]) * (inputImg1[j * width + i] - inputImg[j * width + i]);
		}
	}

	mse1 /= (height * width);
	psnr = mse1 > 0 ? log10(255 * 255 / mse1) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse1, psnr);

	mse2 /= (height * width);
	psnr = mse2 > 0 ? log10(255 * 255 / mse2) : 99.99;
	printf("MSE = %.2lf\nPSNR = %.2lf dB\n", mse2, psnr);

	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}