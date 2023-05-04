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

	unsigned char* inputImg = NULL, * outputImg1 = NULL, * outputImg2 = NULL, * outputImg3 = NULL;;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg3 = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y3 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y4 = (unsigned char*)calloc(width * height, sizeof(unsigned char));

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

	double Gx, Gy, G;

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			Gx = 0;
			Gy = 0;
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					Gx += Y1[(j + y - 1) * width + (i + x - 1)] * sobel_X[y][x];
					Gy += Y1[(j + y - 1) * width + (i + x - 1)] * sobel_Y[y][x];
				}
			}
			G = sqrt(Gx * Gx + Gy * Gy);
			Y2[j * width + i] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));
		}
	}

	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			double Gx = 0, Gy = 0;
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					Gx += Y1[(j + y - 1) * width + (i + x - 1)] * prewitt_X[y][x];
					Gy += Y1[(j + y - 1) * width + (i + x - 1)] * prewitt_Y[y][x];
				}
			}
			double G = sqrt(Gx * Gx + Gy * Gy);
			Y3[j * width + i] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));
		}
	}

	for (int j = 0; j < height - 1; j++) {
		for (int i = 0; i < width - 1; i++) {
			double Gx = 0, Gy = 0;
			for (int y = 0; y < 2; y++) {
				for (int x = 0; x < 2; x++) {
					Gx += Y1[(j + y) * width + i + x] * roberts_X[y][x];
					Gy += Y1[(j + y) * width + i + x] * roberts_Y[y][x];
				}
			}
			double G = sqrt(Gx * Gx + Gy * Gy);
			Y4[j * width + i] = (unsigned char)(G > 255 ? 255 : (G < 0 ? 0 : G));
		}
	}

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

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg3[j * stride + 3 * i + 0] = Y4[j * width + i];
			outputImg3[j * stride + 3 * i + 1] = Y4[j * width + i];
			outputImg3[j * stride + 3 * i + 2] = Y4[j * width + i];
		}
	}

	FILE* outputFile1 = NULL, * outputFile2 = NULL, * outputFile3 = NULL;

	outputFile1 = fopen("AICenterY_Sobel.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile1);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile1);
	fwrite(outputImg1, sizeof(unsigned char), size, outputFile1);

	outputFile2 = fopen("AICenterY_Prewitt.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile2);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile2);
	fwrite(outputImg2, sizeof(unsigned char), size, outputFile2);

	outputFile3 = fopen("AICenterY_Roberts.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile3);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile3);
	fwrite(outputImg3, sizeof(unsigned char), size, outputFile3);

	free(inputImg);
	free(outputImg1);
	free(outputImg2);
	free(outputImg3);
	free(Y1);
	free(Y2);
	free(Y3);
	free(Y4);
	fclose(inputFile);
	fclose(outputFile1);
	fclose(outputFile2);
	fclose(outputFile3);

	return 0;
}