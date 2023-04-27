#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

#define PI 3.1415926535

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

	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	double seta = PI / 3;
	int x0 = 256, y0 = 256;
	int x1, y1, x2, y2;


	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			x1 = i;
			y1 = j;
			x2 = cos(seta) * (x1 - x0) - sin(seta) * (y1 - y0) + x0;
			y2 = sin(seta) * (x1 - x0) + cos(seta) * (y1 - y0) + y0;

			if (x2 > 511 || x2 < 0 || y2 > 511 || y2 < 0);
			else
				Y2[y2 * width + x2] = Y1[i * width + j];
		}
	}

	int dx[4] = { 0, 0, -1, 1 };
	int dy[4] = { -1, 1, 0, 0 };
	int avg = 0;

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			if (Y2[j * width + i] == 0) {
				avg = 0;
				for (int k = 0; k < 4; k++)
					avg += Y2[(j + dy[k]) * width + (i + dx[k])];
				avg = avg / 4;
				Y2[j * width + i] = avg;
			}
		}
	}


	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];
		}
	}

	FILE* outputFile = NULL;
	outputFile = fopen("AICenterY_rotate.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}