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

	unsigned char* inputImg = NULL, * outputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));

	fread(inputImg, sizeof(unsigned char), size, inputFile);

	int Hist[256] = {0};

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Hist[inputImg[j * stride + 3 * i + 0]]++;
			outputImg[j * stride + 3 * i + 0] = 255;
			outputImg[j * stride + 3 * i + 1] = 255;
			outputImg[j * stride + 3 * i + 2] = 255;
			if (i == 0 || i == width - 1) {
				outputImg[j * stride + 3 * i + 0] = 128;
				outputImg[j * stride + 3 * i + 1] = 128;
				outputImg[j * stride + 3 * i + 2] = 128;
			} 
			if (j == 0 || j == width - 1) {
				outputImg[j * stride + 3 * i + 0] = 128;
				outputImg[j * stride + 3 * i + 1] = 128;
				outputImg[j * stride + 3 * i + 2] = 128;
			}
		}
	}

	for (int j = 0; j < 255; j++) {
		for (int i = 0; i < (Hist[j] / 10); i++) {
			outputImg[i * stride + 3 * (j * 2) + 0] = 0;
			outputImg[i * stride + 3 * (j * 2) + 1] = 0;
			outputImg[i * stride + 3 * (j * 2) + 2] = 0;
			outputImg[i * stride + 3 * (j * 2 + 1) + 0] = 0;
			outputImg[i * stride + 3 * (j * 2 + 1) + 1] = 0;
			outputImg[i * stride + 3 * (j * 2 + 1) + 2] = 0;
		}
	}

	FILE* outputFile = fopen("output.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}