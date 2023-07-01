//#define _CRT_SECURE_NO_WARNINGS
//#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>
//#include <windows.h>
//#include <math.h>
//
//int main(int argc, char* argv[]) {
//	BITMAPFILEHEADER bmpFile;
//	BITMAPINFOHEADER bmpInfo;
//	FILE* inputFile = NULL;
//
//	//AICenterY.bmp파일 받아옴
//	inputFile = fopen("AICenterY.bmp", "rb");
//	fread(&bmpFile, sizeof(BITMAPFILEHEADER), 1, inputFile);
//	fread(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, inputFile);
//
//	int width = bmpInfo.biWidth;
//	int height = bmpInfo.biHeight;
//	int size = bmpInfo.biSizeImage;
//	int bitCnt = bmpInfo.biBitCount;
//	int stride = (((bitCnt / 8) * width) + 3) / 4 * 4;
//
//	unsigned char* inputImg = NULL, * outputImg = NULL;
//	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
//	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
//	fread(inputImg, sizeof(unsigned char), size, inputFile);
//
//	unsigned char* Y = (unsigned char*)calloc(width * height, sizeof(unsigned char));
//	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
//	int* e = (int*)calloc(width * height, sizeof(int));
//
//	//입력이미지의 Y값 저장
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			Y[j * width + i] = inputImg[j * stride + 3 * i];
//		}
//	}
//
//	//이전 픽셀 p, 이진으로 저장 bi, 픽셀의 error값 ee
//	int p = 0;
//	char* bi = (char*)malloc(4 * sizeof(char));
//	int ee;
//
//
//	for (int j = 0; j < height; j++) {
//		//첫 픽셀의 p는 128
//		p = 128;
//		//error값인 ee에 현재 픽셀 - p를 저장
//		ee = Y[j * width] - p;
//		//ee를 5로 나눔
//		ee = ee / 5;
//
//		//ee값이 -3보다 작으면 -4로, ee값이 2보다 크면 3으로, 나머지값들은 그대로 error에 저장
//		if (ee < -3) e[j * width] = -4;
//		else if (ee > 2) e[j * width] = 3;
//		else e[j * width] = ee;
//
//		for (int i = 1; i < width; i++) {
//			//이전 픽셀의 p값을 error로 복구
//			p = p + e[j * width + i - 1] * 5;
//			//error값인 ee에 현재 픽셀 - p를 저장
//			ee = Y[j * width + i] - p;
//			//ee를 5로 나눔
//			ee = ee / 5;
//
//			//ee값이 -3보다 작으면 -4로, ee값이 2보다 크면 3으로, 나머지값들은 그대로 error에 저장
//			if (ee < -3) e[j * width + i] = -4;
//			else if (ee > 2) e[j * width + i] = 3;
//			else e[j * width + i] = ee;
//		}
//	}
//
//	//bitstream.txt에 저장하기
//	FILE* text = fopen("bitstream.txt", "w");
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			//각 픽셀의 error값
//			ee = e[j * width + i];
//
//			//조건에 따라서 bi값을 할당함
//			if (ee < -3) bi = "000";
//			else if (ee == -3) bi = "001";
//			else if (ee == -2) bi = "010";
//			else if (ee == -1) bi = "011";
//			else if (ee == 1) bi = "101";
//			else if (ee == 0) bi = "100";
//			else if (ee == 2) bi = "110";
//			else if (ee > 2) bi = "111";
//
//			//bi를 text에 입력
//			fprintf(text, "%s", bi);
//		}
//	}
//
//	//복구하기
//	for (int j = 0; j < height; j++) {
//		//첫 픽셀의 p는 128
//		p = 128;
//		//p에 error * 5를 더해줌으로 원래 픽셀값 복구
//		Y1[j * width] = p + e[j * width] * 5;
//
//		for (int i = 1; i < width; i++) {
//			//이전 픽셀의 p값을 error로 복구
//			p = p + e[j * width + i - 1] * 5;
//			//p에 error * 5를 더해줌으로 원래 픽셀값 복구
//			Y1[j * width + i] = p + e[j * width + i] * 5;
//		}
//	}
//
//	//픽셀값 저장
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			outputImg[j * stride + 3 * i + 0] = Y1[j * width + i];
//			outputImg[j * stride + 3 * i + 1] = Y1[j * width + i];
//			outputImg[j * stride + 3 * i + 2] = Y1[j * width + i];
//		}
//	}
//
//	//reconEncY 파일 출력
//	FILE* outputFile = fopen("reconEncY.bmp", "wb");
//	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
//	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
//	fwrite(outputImg, sizeof(unsigned char), size, outputFile);
//
//	free(inputImg);
//	free(outputImg);
//	fclose(inputFile);
//	fclose(outputFile);
//
//
//	return 0;
//}