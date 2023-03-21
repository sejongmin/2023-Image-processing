#pragma warning (disable : 4996)
#include <stdio.h>
#include <stdlib.h>

int main() {
	FILE* fp1 = fopen("input.txt", "rb");
	FILE* fp2 = fopen("output.txt", "wb");

	unsigned char* ch = (unsigned char*)malloc(sizeof(unsigned char));

	for (int i = 0; i < 12; i++) {
		fread(ch, sizeof(unsigned char), 1, fp1);
		fwrite(ch, sizeof(unsigned char), 1, fp2);
	}

	free(ch);
	fclose(fp1); fclose(fp2);

	return 0;
}