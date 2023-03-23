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

	BITMAPFILEHEADER bmpFile_;
	BITMAPINFOHEADER bmpInfo_;
	FILE* inputFile_ = NULL;

	inputFile_ = fopen("mask.bmp", "rb");
	fread(&bmpFile_, sizeof(BITMAPFILEHEADER), 1, inputFile_);
	fread(&bmpInfo_, sizeof(BITMAPINFOHEADER), 1, inputFile_);

	unsigned char* inputImg = NULL, * outputImg = NULL;
	unsigned char* inputImg_ = NULL;

	inputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	inputImg_ = (unsigned char*)calloc(size, sizeof(unsigned char));
	//outputImg1 = (unsigned char*)calloc(size, sizeof(unsigned char));
	//outputImg2 = (unsigned char*)calloc(size, sizeof(unsigned char));
	outputImg = (unsigned char*)calloc(size, sizeof(unsigned char));
	fread(inputImg, sizeof(unsigned char), size, inputFile);
	fread(inputImg_, sizeof(unsigned char), size, inputFile_);


	double Y, Cb, Cr, R, G, B, I, Diff_Y, Y1;
	int mask;

	/*for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
			Cb = -0.169 * inputImg[j * stride + 3 * i + 2] - 0.331 * inputImg[j * stride + 3 * i + 1] + 0.500 * inputImg[j * stride + 3 * i + 0];
			Cr = 0.500 * inputImg[j * stride + 3 * i + 2] - 0.419 * inputImg[j * stride + 3 * i + 1] - 0.0813 * inputImg[j * stride + 3 * i + 0];

			R = Y + 1.402 * Cr;
			G = Y - 0.714 * Cr - 0.344 * Cb;
			B = Y + 1.772 * Cb;

			I = (unsigned char)((inputImg[j * stride + 3 * i + 2] + inputImg[j * stride + 3 * i + 1] + inputImg[j * stride + 3 * i + 0]) / 3);

			outputImg1[j * stride + 3 * i + 2] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg1[j * stride + 3 * i + 1] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg1[j * stride + 3 * i + 0] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));

			outputImg2[j * stride + 3 * i + 2] = (unsigned char)(I > 255 ? 255 : (I < 0 ? 0 : I));
			outputImg2[j * stride + 3 * i + 1] = (unsigned char)(I > 255 ? 255 : (I < 0 ? 0 : I));
			outputImg2[j * stride + 3 * i + 0] = (unsigned char)(I > 255 ? 255 : (I < 0 ? 0 : I));

			Diff_Y = (Y - I)*(Y - I);

			outputImg[j * stride + 3 * i + 2] = (unsigned char)(Diff_Y > 255 ? 255 : (Diff_Y < 0 ? 0 : Diff_Y));
			outputImg[j * stride + 3 * i + 1] = (unsigned char)(Diff_Y > 255 ? 255 : (Diff_Y < 0 ? 0 : Diff_Y));
			outputImg[j * stride + 3 * i + 0] = (unsigned char)(Diff_Y > 255 ? 255 : (Diff_Y < 0 ? 0 : Diff_Y));
		};
	}*/

	/*FILE* outputFile1 = fopen("Output_Y.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile1);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile1);
	fwrite(outputImg1, sizeof(unsigned char), size, outputFile1);

	FILE* outputFile2 = fopen("Output_I.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile2);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile2);
	fwrite(outputImg2, sizeof(unsigned char), size, outputFile2);*/

	for (int j = 0; j < height; j++) {
		for (int i = 0; i < width; i++) {
			Y1 = 0.299 * inputImg[j * stride + 3 * i + 2] + 0.587 * inputImg[j * stride + 3 * i + 1] + 0.114 * inputImg[j * stride + 3 * i + 0];
			mask = inputImg_[j * stride + 3 * i + 2] + inputImg_[j * stride + 3 * i + 1] + inputImg_[j * stride + 3 * i + 0];
			mask = mask > 0 ? 1 : 0;
			
			Y = Y1 * mask;

			outputImg[j * stride + 3 * i + 2] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg[j * stride + 3 * i + 1] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
			outputImg[j * stride + 3 * i + 0] = (unsigned char)(Y > 255 ? 255 : (Y < 0 ? 0 : Y));
		}
	}

	FILE* outputFile = fopen("Output_mask_2.bmp", "wb");
	fwrite(&bmpFile, sizeof(BITMAPFILEHEADER), 1, outputFile);
	fwrite(&bmpInfo, sizeof(BITMAPINFOHEADER), 1, outputFile);
	fwrite(outputImg, sizeof(unsigned char), size, outputFile);


	free(inputImg);
	free(inputImg_);
	free(outputImg);
	fclose(inputFile);
	fclose(inputFile_);
	fclose(outputFile);

	return 0;
}