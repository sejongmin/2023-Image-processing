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
//	//헤더용으로 GateY.bmp 받아옴
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
//	unsigned char * outputImg = NULL;
//	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
//
//	unsigned char* Y1 = (unsigned char*)calloc(width * height, sizeof(unsigned char));
//	int* e = (int*)calloc(width * height, sizeof(int));
//
//	//이전 픽셀 p, 이진으로 저장 bi, 픽셀의 error값 ee
//	int p = 0;
//	char* bi = (char*)malloc(sizeof(char) * 4);
//	int ee;
//
//	//bitstream.txt파일 읽어옴
//	FILE* text = fopen("bitstream.txt", "r");
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			//3글자(3비트)씩 읽어옴
//			fscanf(text, "%3s", bi);
//
//			//조건에 맞게 ee 복구
//			if (strcmp(bi, "000") == 0) ee = -4;
//			else if (strcmp(bi, "001") == 0) ee = -3;
//			else if (strcmp(bi, "010") == 0) ee = -2;
//			else if (strcmp(bi, "011") == 0) ee = -1;
//			else if (strcmp(bi, "100") == 0) ee = 0;
//			else if (strcmp(bi, "101") == 0) ee = 1;
//			else if (strcmp(bi, "110") == 0) ee = 2;
//			else if (strcmp(bi, "111") == 0) ee = 3;
//
//			//error에 ee를 할당
//			e[j * width + i] = ee;
//		}
//	}
//
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
//	//reconDecY 파일 출력
//	FILE* outputFile = fopen("reconDecY.bmp", "wb");
//	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
//	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
//	fwrite(outputImg, sizeof(unsigned char), size, outputFile);
//
//	free(outputImg);
//	fclose(outputFile);
//
//	return 0;
//}