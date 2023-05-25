#pragma warning	(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>


int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;

	inputFile = fopen("AICenterY_Dilation.bmp", "rb");
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

	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y2[j * width + i] = (unsigned char)Y1[j * width + i] > 170 ? 255 : 0;
		}
	}*/

	/*erosion*/
	for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			int edge = 1;
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					if (Y1[(j + y - 1) * width + (i + x - 1)] == 0)
						edge = 0;
				}
			}
			if (edge == 1)
				Y2[j * width + i] = 255;
		}
	}
	
	/*Dilation*/
	/*for (int j = 1; j < height - 1; j++) {
		for (int i = 1; i < width - 1; i++) {
			int edge = 0;
			for (int y = 0; y < 3; y++) {
				for (int x = 0; x < 3; x++) {
					if (Y1[(j + y - 1) * width + (i + x - 1)] == 255) 
						edge = 1;
				}
			}
			if (edge == 1)
				Y2[j * width + i] = 255;
		}
	}*/


	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			outputImg[j * stride + 3 * i + 0] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 1] = Y2[j * width + i];
			outputImg[j * stride + 3 * i + 2] = Y2[j * width + i];
		}
	}

	FILE* outputFile = NULL;

	outputFile = fopen("AICenterY_Closing.bmp", "wb");
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