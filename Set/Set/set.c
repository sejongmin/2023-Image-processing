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

	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i + 0];
		}
	}

	int sobel_X[3][3] = { {-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1} };
	int sobel_Y[3][3] = { {-1, -2, -1}, {0, 0, 0}, {1, 2, 1} };
	int prewitt_X[3][3] = { {-1, 0, 1}, {-1, 0, 1}, {-1, 0, 1} };
	int prewitt_Y[3][3] = { {-1, -1, -1}, {0, 0, 0}, {1, 1, 1} };
	int roberts_X[2][2] = { {-1, 0}, {0, 1} };
	int roberts_Y[2][2] = { {0, -1}, {1, 0} };

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			double Gx_sobel = 0, Gy_sobel = 0, G_sobel;
			double Gx_prewitt = 0, Gy_prewitt = 0, G_prewitt;
			double Gx_roberts = 0, Gy_roberts = 0, G_roberts;
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					Gx_sobel += Y1[(j + y - 1) * width + (i + x - 1)] * sobel_X[y][x];
					Gy_sobel += Y1[(j + y - 1) * width + (i + x - 1)] * sobel_Y[y][x];

					Gx_prewitt += Y1[(j + y - 1) * width + (i + x - 1)] * prewitt_X[y][x];
					Gy_prewitt += Y1[(j + y - 1) * width + (i + x - 1)] * prewitt_Y[y][x];
				}
			}
			for (int y = 0; y < 2; y++) {
				for (int x = 0; x < 2; x++) {
					Gx_roberts += Y1[(j + y) * width + i + x] * roberts_X[y][x];
					Gy_roberts += Y1[(j + y) * width + i + x] * roberts_Y[y][x];
				}
			}
			G_sobel = sqrt(Gx_sobel * Gx_sobel + Gy_sobel * Gy_sobel);
			G_prewitt = sqrt(Gx_prewitt * Gx_prewitt + Gy_prewitt * Gy_prewitt);
			G_roberts = sqrt(Gx_roberts * Gx_roberts + Gy_roberts * Gy_roberts);

			if (G_sobel < 200)
				Y2[j * width + i] = 255;
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

	outputFile = fopen("AICenterY_Sobel_Complement.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);

	
	free(inputImg);
	free(outputImg);
	free(Y1);
	free(Y2);
	fclose(inputFile);
	fclose(outputFile);

	return 0;
}