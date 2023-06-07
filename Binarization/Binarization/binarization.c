#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;

	inputFile = fopen("AICenterY.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width, stride, height, size, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	unsigned char* Y = (unsigned char*)calloc(size, sizeof(unsigned char));
	unsigned char* Y1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	int* e = (int*)calloc(size, sizeof(int));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y[j * width + i] = inputImg[j * stride + 3 * i];
		}
	}

	int p = 0;
	char* bi = (char*)malloc(sizeof(char) * 4);
	int ee;
	/*
	for (int j = 0; j < height; j++) {
		p = 128;
		ee = Y[j * width] - p;
		ee = ee / 5;

		if (ee < -3) e[j * width] = -4;
		else if (ee > 2) e[j * width] = 3;
		else e[j * width] = ee;

		for (int i = 1; i < width; i++) {
			p = p + e[j * width + i - 1] * 5;
			ee = Y[j * width + i] - p;
			ee = ee / 5;

			if (ee < -3) e[j * width + i] = -4;
			else if (ee > 2) e[j * width + i] = 3;
			else e[j * width + i] = ee;
		}
	}

	FILE* text = fopen("bitstream.txt", "w");
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			ee = e[j * width + i];

			if (ee < -3) bi = "000";
			else if (ee == -3) bi = "001";
			else if (ee == -2) bi = "010";
			else if (ee == -1) bi = "011";
			else if (ee == 1) bi = "101";
			else if (ee == 0) bi = "100";
			else if (ee == 2) bi = "110";
			else if (ee > 2) bi = "111";

			fprintf(text, "%s ", bi);
		}
	}*/

	FILE* text = fopen("bitstream.txt", "r");
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			fscanf(text, "%s", bi);

			if (strcmp(bi, "000") == 0) ee = -4;
			else if (strcmp(bi, "001") == 0) ee = -3;
			else if (strcmp(bi, "010") == 0) ee = -2;
			else if (strcmp(bi, "011") == 0) ee = -1;
			else if (strcmp(bi, "100") == 0) ee = 0;
			else if (strcmp(bi, "101") == 0) ee = 1;
			else if (strcmp(bi, "110") == 0) ee = 2;
			else if (strcmp(bi, "111") == 0) ee = 3;

			e[j * width + i] = ee;
		}
	}

	for (int j = 0; j < height; j++) {
		p = 128;
		Y1[j * width] = p + e[j * width] * 5;
		for (int i = 1; i < width; i++) {
			p = p + e[j * width + i - 1] * 5;
			Y1[j * width + i] = p + e[j * width + i] * 5;
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

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = Y1[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y1[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y1[j * width + i];
		}
	}

	FILE* outputFile = fopen("reconDecY.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}