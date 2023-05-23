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

	//�� ������ �Ǵ� �ȼ��κ��� ����ִ� �ȼ�3���� ���� (75%, 25%) / (50%, 50%) / (25%, 75%)�� ����ġ
	double upFilter[3][2] = { {3 / 4.0, 1 / 4.0}, {2 / 4.0, 2 / 4.0}, {1 / 4.0, 3 / 4.0} };

	//512*512���� 4ĭ���� 128 * 128�� ���� �ȼ����� �Է�
	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y2[(j << ratio) * width1 + (i << ratio)] = Y1[j * width + i];
		}
	}

	//nearest neighbor ����� �̿��ؼ� �ٿ������ ó��
	for (int j = 0; j < height1; j++) {
		for (int i = 0; i < width1; i++) {
			if (j >= height1 - 4 || i >= width1 - 4)
				Y2[j * width1 + i] = Y1[(j >> ratio) * width + (i >> ratio)];
		}
	}

	//�ٿ������ �̹� ó�������Ƿ� �ٿ������ ������ ������ �ȼ�
	//4ĭ���� �Էµ� �ȼ��� �߰��� �ִ� ������ ���͸� �̿��ؼ� ä��(1�ȼ��� �ߺ��Ǿ��ִ� 5*5ĭ�� �׵θ� �κ�)
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

	//�ٿ������ �̹� ó�������Ƿ� �ٿ������ ������ ������ �ȼ�
	//5*5ĭ�� �׵θ��� ���� ���͸� �����ؼ� 3*3�� �� �ȼ��� ä��
	for (int j = 0; j < height - 1; j++) {
		for (int i = 0; i < width - 1; i++) {
			//�ƽ���Ʈ(������ 0 ~ 1/7)�� �ϴ�(������ 7/8 ~ 1) �κ��� �����ȼ��� bilinear�� �̿��ؼ� ä��
			if (j < height / 7.0 || j > height * 7 / 8.0) {
				for (int y = 0; y < 3; y++) {
					//�� ���� �ȼ��� �������� ������
					int lp = Y2[((j << ratio) + y + 1) * width1 + (i << ratio)];
					int rp = Y2[((j << ratio) + y + 1) * width1 + ((i + 1) << ratio)];
					for (int x = 0; x < 3; x++) {
						//���ذ����ȼ� ���� ���͸� �̿��� �°� bilinear�� �ȼ��� ä��
						Y2[((j << ratio) + y + 1) * width1 + (i << ratio) + x + 1] = upFilter[x][0] * lp + upFilter[x][1] * rp;
					}
				}
			}
			//�ƽ���Ʈ�� �ϴ��� ������ �κ��� �����ȼ��� bilinear�� �̿��ؼ� ä��
			else {
				for (int y = 0; y < 3; y++) {
					//�� ���� �ȼ��� �������� ������
					int up = Y2[(j << ratio) * width1 + (i << ratio) + y + 1];
					int dp = Y2[((j + 1) << ratio) * width1 + (i << ratio) + y + 1];
					for (int x = 0; x < 3; x++) {
						//���ؼ����ȼ� ���� ���͸� �̿��� bilinear�� �� �ȼ��� ä��
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