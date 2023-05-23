#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <math.h>

int main(int argc, char* argv[]) {
	BITMAPFILEHEADER bmpFile;
	BITMAPINFOHEADER bmpInfo;
	FILE* inputFile = NULL;

	inputFile = fopen("test.bmp", "rb");
	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);

	int width = bmpInfo.biWidth;
	int height = bmpInfo.biHeight;
	int size = bmpInfo.biSizeImage;
	int bitCnt = bmpInfo.biBitCount;
	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;

	int ratio = 2;
	int width1 = bmpInfo.biWidth << ratio;
	int height1 = bmpInfo.biHeight << ratio;
	int stride1 = (((bitCnt / 8) * width1) + 3) / 4 * 4;
	int size1 = stride1 * height1;

	unsigned char * outputImg = NULL, * inputImg = NULL;
	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size1, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);

	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
	unsigned char* Y2 = (unsigned char*)calloc(width1 * height1, sizeof(unsigned char));

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1[j * width + i] = inputImg[j * stride + 3 * i];
		}
	}

	//양 기준이 되는 픽셀로부터 비어있는 픽셀3개를 각각 (75%, 25%) / (50%, 50%) / (25%, 75%)의 가중치
	double upFilter[3][2] = { {3 / 4.0, 1 / 4.0}, {2 / 4.0, 2 / 4.0}, {1 / 4.0, 3 / 4.0} };

	//512*512영상에 4칸마다 128 * 128의 영상 픽셀값을 입력
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y2[(j << ratio) * width1 + (i << ratio)] = Y1[j * width + i];
		}
	}

	//nearest neighbor 방법을 이용해서 바운더리를 처리
	for (int j = 0; j < height1; j++) {
		for (int i = 0; i < width1; i++) {
			if (j >= height1 - 4 || i >= width1 - 4)
				Y2[j * width1 + i] = Y1[(j >> ratio) * width + (i >> ratio)];
		}
	}

	//바운더리는 이미 처리했으므로 바운더리를 제외한 나머지 픽셀
	//4칸마다 입력된 픽셀값 중간에 있는 값들을 필터를 이용해서 채움(1픽셀씩 중복되어있는 5*5칸의 테두리 부분)
	for (int j = 0; j < height - 1; j++) {
		for (int i = 0; i < width - 1; i++) {
			int lp = Y1[j * width + i];
			int rp = Y1[j * width + i + 1];
			int dp = Y1[(j + 1) * width + i];
			for (int x = 0; x < 3; x++) {
				Y2[(j << ratio) * width1 + ((i << ratio) + x + 1)] = upFilter[x][0] * lp + upFilter[x][1] * rp;
				Y2[((j << ratio) + x + 1) * width1 + (i << ratio)] = upFilter[x][0] * lp + upFilter[x][1] * dp;
			}
		}
	}

	//바운더리는 이미 처리했으므로 바운더리를 제외한 나머지 픽셀
	//5*5칸의 테두리의 값의 필터를 적용해서 3*3의 빈 픽셀을 채움
	for (int j = 0; j < height - 1; j++) {
		for (int i = 0; i < width - 1; i++) {
			//아스팔트(세로의 0 ~ 1/7)와 하늘(세로의 7/8 ~ 1) 부분은 가로픽셀의 bilinear을 이용해서 채움
			if (j < height / 7.0 || j > height * 7 / 8.0) {
				for (int y = 0; y < 3; y++) {
					//양 가로 픽셀을 기준으로 가져옴
					int lp = Y2[((j << ratio) + y + 1) * width1 + (i << ratio)];
					int rp = Y2[((j << ratio) + y + 1) * width1 + ((i + 1) << ratio)];
					for (int x = 0; x < 3; x++) {
						//기준가로픽셀 값의 필터를 이용해 맞게 bilinear로 픽셀을 채움
						Y2[((j << ratio) + y + 1) * width1 + (i << ratio) + x + 1] = upFilter[x][0] * lp + upFilter[x][1] * rp;
					}
				}
			}
			//아스팔트와 하늘을 제외한 부분은 세로픽셀의 bilinear을 이용해서 채움
			else {
				for (int y = 0; y < 3; y++) {
					//양 세로 픽셀을 기준으로 가져옴
					int up = Y2[(j << ratio) * width1 + (i << ratio) + y + 1];
					int dp = Y2[((j + 1) << ratio) * width1 + (i << ratio) + y + 1];
					for (int x = 0; x < 3; x++) {
						//기준세로픽셀 값의 필터를 이용해 bilinear로 빈 픽셀을 채움
						Y2[((j << ratio) + x + 1) * width1 + (i << ratio) + y + 1] = upFilter[x][0] * up + upFilter[x][1] * dp;
					}
				}
			}
		}
	}

	for (int j = 0; j < height1; j++) {
		for (int i = 0; i < width1; i++) {
			outputImg[j * stride1 + 3 * i + 0] = Y2[j * width1 + i];
			outputImg[j * stride1 + 3 * i + 1] = Y2[j * width1 + i];
			outputImg[j * stride1 + 3 * i + 2] = Y2[j * width1 + i];

		}
	}

	FILE* outputFile = fopen("19011817.bmp", "wb");
	bmpInfo.biWidth = width1;
	bmpInfo.biHeight = height1;
	bmpInfo.biSizeImage = size1;
	bmpFile.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + size1;
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size1, outputFile);

	free(Y1);
	free(Y2);
	free(inputImg);
	free(outputImg);
	fclose(outputFile);
	fclose(inputFile);
	
	return 0;
}