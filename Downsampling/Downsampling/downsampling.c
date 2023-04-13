#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

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


	int ratio = 2;
	int width2 = bmpInfo.biWidth >> ratio;
	int height2 = bmpInfo.biHeight >> ratio;
	int stride2 = (((bitCnt / 8) * width2) + 3) / 4 * 4;
	int size2 = stride2 * height2;
	printf("W: %d(%d)\nH: %d\nS: %d\nD: %d\n\n", width2, stride2, height2, size2, bitCnt);

	unsigned char* inputImg = NULL, * outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size2, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width2 * height2, sizeof(unsigned char));

	int sum, avg;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
		}
	}
	//subsampling
	/*for (int j = 0; j < height2; j++) {
		for (int i = 0; i < width2; i++) {
			Y2[j * width2 + i] = Y1[(j << ratio) * width + (i << ratio)];
		}
	}*/

	//average
	for (int j = 0; j < height2; j++) {
		for (int i = 0; i < width2; i++) {
			sum = 0;
			sum += Y1[(j << ratio) * width + (i << ratio)];
			sum += Y1[(j << ratio) * width + (i << ratio) + 1];
			sum += Y1[((j << ratio) + 1) * width + (i << ratio)];
			sum += Y1[((j << ratio) + 1) * width + (i << ratio) + 1];
			avg = (unsigned char)(sum / 4);

			Y2[j * width2 + i] = avg;
		}
	}

	for (int j = 0; j < height2; j++) {
		for (int i = 0; i < width2; i++) {
			outputImg[j * stride2 + 3 * i + 0] = Y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 1] = Y2[j * width2 + i];
			outputImg[j * stride2 + 3 * i + 2] = Y2[j * width2 + i];
		}
	}


	FILE* outputFile = fopen("output_avg.bmp", "wb");
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