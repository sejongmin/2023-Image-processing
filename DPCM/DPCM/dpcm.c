#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
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
	
	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i == 0) {
				p = 128;
				e[j * width + i] = Y[j * width + i] - p;
			}
			else {
				p += e[j * width + i - 1];
				e[j * width + i] = Y[j * width + i] - p;
			}
			e[j * width + i] = e[j * width + i] / 5 * 5;
		}
	}

	FILE* text = fopen("DPCM.txt", "w");
	int* ch = (int*)malloc(sizeof(int));
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			ch = e[j * width + i] / 5;
			fprintf(text, "%d ", ch);
		}
	}*/
	
	FILE* text = fopen("DPCM.txt", "r");
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			fscanf(text, "%d", &e[j * width + i]);
			e[j * width + i] *= 5;
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			if (i == 0) {
				p = 128;
				Y1[j * width + i] = p + e[j * width + i];
			}
			else {
				p = p + e[j * width + i - 1];
				Y1[j * width + i] = p + e[j * width + i];
			}
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