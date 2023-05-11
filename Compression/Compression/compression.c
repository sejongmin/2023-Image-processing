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

	unsigned char* inputImg = NULL, * outputImg1 = NULL, * outputImg2 = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	FILE* text_8 = NULL, * text_4 = NULL;
	text_8 = fopen("bitstream_8.txt", "rb");
	text_4 = fopen("bitstream_4.txt", "rb");

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			fscanf(text_8, "%d ", &Y2[j * width + i]);
			fscanf(text_4, "%d ", &Y3[j * width + i]);
			Y3[j * width + i] *= 16;
		}
	}

	/*
	int* ch_8 = (int*)malloc(sizeof(int));
	int* ch_4 = (int*)malloc(sizeof(int));
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			ch_8 = Y1[j * width + i];
			ch_4 = Y1[j * width + i] / 16;
			fprintf(text_8, "%d ", ch_8);
			fprintf(text_4, "%d ", ch_4);
		}
	}*/

	double mse1 = 0, mse2 = 0, psnr1, psnr2;
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			mse1 += (double)((Y2[j * width + i] - Y1[j * width + i]) * (Y2[j * width + i] - Y1[j * width + i]));
			mse2 += (double)((Y3[j * width + i] - Y1[j * width + i]) * (Y3[j * width + i] - Y1[j * width + i]));
		}
	}
	mse1 /= (width * height);
	mse2 /= (width * height);
	psnr1 = mse1 != 0.0 ? 10.0 * log10(255 * 255 / mse1) : 99.99;
	psnr2 = mse2 != 0.0 ? 10.0 * log10(255 * 255 / mse2) : 99.99;

	printf("8bit depth\nMSE = %.2f\nPSNR = %.2f dB\n\n", mse1, psnr1);
	printf("4bit depth\nMSE = %.2f\nPSNR = %.2f dB\n\n", mse2, psnr2);

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg1[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg1[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg1[j * stride + 3 * i + 2] = Y2[j * width + i];
		}
	}

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg2[j * stride + 3 * i + 0] = Y3[j * width + i];
			outputImg2[j * stride + 3 * i + 1] = Y3[j * width + i];
			outputImg2[j * stride + 3 * i + 2] = Y3[j * width + i];
		}
	}

	FILE* outputFile1 = NULL, * outputFile2 = NULL;

	outputFile1 = fopen("reconstruction_8.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile1);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile1);
	fwrite(outputImg1, sizeof(unsigned char), size, outputFile1);

	outputFile2 = fopen("reconstruction_4.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile2);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile2);
	fwrite(outputImg2, sizeof(unsigned char), size, outputFile2);

	/*fclose(text_8), fclose(text_4);
	free(Y1); free(Y2); free(Y3); */
	free(inputImg); free(outputImg1); free(outputImg2);
	fclose(inputFile); fclose(outputFile1); fclose(outputFile2);

	return 0;
}