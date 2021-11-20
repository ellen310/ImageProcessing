#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define WIDTH 374
#define HEIGHT 374
#define N 7
#define M 7
#define T1 50 
#define T2 28 
#define T3 15 
#define q 28 
#pragma warning(disable:4996)


int main() {
	FILE* fp;

	unsigned char* InImg = NULL;
	float* dy = NULL;
	float* dx = NULL;
	unsigned char* Idy = NULL;
	unsigned char* Idx = NULL;
	unsigned char* OutImg = NULL;

	unsigned char mask[N][M] = { {0,0,1,1,1,0,0},
								 {0,1,1,1,1,1,0},
								 {1,1,1,1,1,1,1},
								 {1,1,1,1,1,1,1},
								 {1,1,1,1,1,1,1},
								 {0,1,1,1,1,1,0},
								 {0,0,1,1,1,0,0} };

	InImg = (unsigned char*)malloc(WIDTH * HEIGHT);
	OutImg = (unsigned char*)malloc(WIDTH * HEIGHT);
	memset(InImg, 0, WIDTH * HEIGHT);
	fp = fopen("lena_grayscale_374x374.raw", "rb");
	fread(InImg, 1, WIDTH * HEIGHT, fp);
	fclose(fp);

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		int hh = i / WIDTH;
		int ww = i % WIDTH;
		int ConImg = 0;
		int Sum = 0;
		int C = 0;

		if (hh >= N / 2 && hh < HEIGHT - N / 2 && ww >= M / 2 && ww < WIDTH - M / 2) {
			for (int ii = -N / 2; ii < N / 2 + 1; ii++) {
				for (int jj = -M / 2; jj < M / 2 + 1; jj++) {
					ConImg = InImg[(hh + ii) * WIDTH + ww + jj] * mask[ii + N / 2][jj + M / 2];
					if (abs(ConImg - InImg[i]) < T1) Sum++;
				}
			}
		}

		if (Sum <= T2) C = q - Sum;
		else C = 0;

		if (C > T3) {
			OutImg[i] = 255;
		}
		else OutImg[i] = 0;
	}

	fp = fopen("lenna_susan.raw", "wb");
	fwrite(OutImg, 1, WIDTH * HEIGHT, fp);
	fclose(fp);

	return 0;
}