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

	double Y, Cb, Cr, R, G, B, I;

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
			Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
			Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];
			
			Y += 100;

			R = Y + 1.402 * Cr;
			G = Y - 0.714 * Cr - 0.344 * Cb;
			B = Y + 1.772 * Cb;

			I = (unsigned char)((inputImg[j * stride + 3 * i + 2] + inputImg[j * stride + 3 * i + 1] + inputImg[j * stride + 3 * i + 0]) / 3);

			outputImg[j * stride + 3 * i + 2] = (unsigned char)(R > 255 ? 255 : (R < 0 ? 0 : R));
			outputImg[j * stride + 3 * i + 1] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));
			outputImg[j * stride + 3 * i + 0] = (unsigned char)(B > 255 ? 255 : (B < 0 ? 0 : B));
		};
	}


	FILE* outputFile = fopen("Output_Light.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);


	free(inputImg);
	free(outputImg);
	fclose(inputFile);
	fclose(outputFile);
	


	return 0;
}
