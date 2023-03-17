#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;

	inputFile = fopen("AICenter.bmp", "rb");
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

	//Original Copy
	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
		}
	}*/

	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i < 100 && j < 100) {
				outputImg[j * stride + 3 * i + 0] = 0;
				outputImg[j * stride + 3 * i + 1] = 0;
				outputImg[j * stride + 3 * i + 2] = 0;
				continue;
			}
			if (i > 400 && j > 400) {
				outputImg[j * stride + 3 * i + 0] = 255;
				outputImg[j * stride + 3 * i + 1] = 255;
				outputImg[j * stride + 3 * i + 2] = 255;
				continue;
			}
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0];
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2];
		}
	}*/

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 2];	//output B
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1];	//output G
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 0];	//output R
		}
	}

	//FILE* outputFile = fopen("Output.bmp", "wb");
	//FILE* outputFile = fopen("Output(1).bmp", "wb");
	FILE* outputFile = fopen("Output(2).bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(outputImg);
	free(inputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}