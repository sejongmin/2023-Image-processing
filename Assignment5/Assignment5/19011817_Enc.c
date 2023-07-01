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
//	//AICenterY.bmp���� �޾ƿ�
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
//	//�Է��̹����� Y�� ����
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			Y[j * width + i] = inputImg[j * stride + 3 * i];
//		}
//	}
//
//	//���� �ȼ� p, �������� ���� bi, �ȼ��� error�� ee
//	int p = 0;
//	char* bi = (char*)malloc(4 * sizeof(char));
//	int ee;
//
//
//	for (int j = 0; j < height; j++) {
//		//ù �ȼ��� p�� 128
//		p = 128;
//		//error���� ee�� ���� �ȼ� - p�� ����
//		ee = Y[j * width] - p;
//		//ee�� 5�� ����
//		ee = ee / 5;
//
//		//ee���� -3���� ������ -4��, ee���� 2���� ũ�� 3����, ������������ �״�� error�� ����
//		if (ee < -3) e[j * width] = -4;
//		else if (ee > 2) e[j * width] = 3;
//		else e[j * width] = ee;
//
//		for (int i = 1; i < width; i++) {
//			//���� �ȼ��� p���� error�� ����
//			p = p + e[j * width + i - 1] * 5;
//			//error���� ee�� ���� �ȼ� - p�� ����
//			ee = Y[j * width + i] - p;
//			//ee�� 5�� ����
//			ee = ee / 5;
//
//			//ee���� -3���� ������ -4��, ee���� 2���� ũ�� 3����, ������������ �״�� error�� ����
//			if (ee < -3) e[j * width + i] = -4;
//			else if (ee > 2) e[j * width + i] = 3;
//			else e[j * width + i] = ee;
//		}
//	}
//
//	//bitstream.txt�� �����ϱ�
//	FILE* text = fopen("bitstream.txt", "w");
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			//�� �ȼ��� error��
//			ee = e[j * width + i];
//
//			//���ǿ� ���� bi���� �Ҵ���
//			if (ee < -3) bi = "000";
//			else if (ee == -3) bi = "001";
//			else if (ee == -2) bi = "010";
//			else if (ee == -1) bi = "011";
//			else if (ee == 1) bi = "101";
//			else if (ee == 0) bi = "100";
//			else if (ee == 2) bi = "110";
//			else if (ee > 2) bi = "111";
//
//			//bi�� text�� �Է�
//			fprintf(text, "%s", bi);
//		}
//	}
//
//	//�����ϱ�
//	for (int j = 0; j < height; j++) {
//		//ù �ȼ��� p�� 128
//		p = 128;
//		//p�� error * 5�� ���������� ���� �ȼ��� ����
//		Y1[j * width] = p + e[j * width] * 5;
//
//		for (int i = 1; i < width; i++) {
//			//���� �ȼ��� p���� error�� ����
//			p = p + e[j * width + i - 1] * 5;
//			//p�� error * 5�� ���������� ���� �ȼ��� ����
//			Y1[j * width + i] = p + e[j * width + i] * 5;
//		}
//	}
//
//	//�ȼ��� ����
//	for (int j = 0; j < height; j++) {
//		for (int i = 0; i < width; i++) {
//			outputImg[j * stride + 3 * i + 0] = Y1[j * width + i];
//			outputImg[j * stride + 3 * i + 1] = Y1[j * width + i];
//			outputImg[j * stride + 3 * i + 2] = Y1[j * width + i];
//		}
//	}
//
//	//reconEncY ���� ���
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