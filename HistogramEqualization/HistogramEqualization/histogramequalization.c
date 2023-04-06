#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;

	inputFile = fopen("AICenterY_output.bmp", "rb");
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

	int Hist[256] = { 0 };
	int sumHist[256] = { 0 };

	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = inputImg[j * stride + 3 * i + 0] / 2;
			outputImg[j * stride + 3 * i + 1] = inputImg[j * stride + 3 * i + 1] / 2;
			outputImg[j * stride + 3 * i + 2] = inputImg[j * stride + 3 * i + 2] / 2;
		}
	}*/

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

	sumHist[0] = Hist[0];

	for (int i = 1; i < 256; i++) {
			sumHist[i] = sumHist[i - 1] + Hist[i];
	}

	printf("S");

	for (int i = 0; i < 256; i++) {
		for (int j = 0; j < sumHist[i] / 512; j++) {
			outputImg[j * stride + 3 * (i * 2) + 0] = 0;
			outputImg[j * stride + 3 * (i * 2) + 1] = 0;
			outputImg[j * stride + 3 * (i * 2) + 2] = 0;
			outputImg[j * stride + 3 * (i * 2 + 1) + 0] = 0;
			outputImg[j * stride + 3 * (i * 2 + 1) + 1] = 0;
			outputImg[j * stride + 3 * (i * 2 + 1) + 2] = 0;
		}
	}

	printf("F");


	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = (255 * sumHist[inputImg[j * stride + 3 * i + 0]]) / (width * height);
			outputImg[j * stride + 3 * i + 1] = (255 * sumHist[inputImg[j * stride + 3 * i + 1]]) / (width * height);
			outputImg[j * stride + 3 * i + 2] = (255 * sumHist[inputImg[j * stride + 3 * i + 2]]) / (width * height);
		}
	}*/

	FILE* outputFile = fopen("sumHistogram_output.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}