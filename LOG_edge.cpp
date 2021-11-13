#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

#define WIDTH 374
#define HEIGHT 374
#define N 3
#define M 3
#define L 1
#define TH 150
#define Th 1000
#pragma warning(disable:4996)

int main() {
	FILE* fp;
	unsigned char* InImg = NULL;
	float* LOG = NULL;
	unsigned char* OutImg = NULL;

	float mask[N][M] = { {0.4038, 0.8021, 0.4038},

						  {0.8021, -4.8233, 0.8021},

						  {0.4038, 0.8021, 0.4038} };


	InImg = (unsigned char*)malloc(WIDTH * HEIGHT);
	LOG = (float*)malloc(WIDTH * HEIGHT * 4);
	OutImg = (unsigned char*)malloc(WIDTH * HEIGHT);

	memset(InImg, 0, WIDTH * HEIGHT);
	memset(OutImg, 0, WIDTH * HEIGHT);

	fp = fopen("lena_grayscale_374x374.raw", "rb");
	fread(InImg, 1, WIDTH * HEIGHT, fp);
	fclose(fp);

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		int hh = i / WIDTH;
		int ww = i % WIDTH;
		float ConImg = 0.0;

		if (hh >= N / 2 && hh < HEIGHT - N / 2 && ww >= M / 2 && ww < WIDTH - M / 2) {
			for (int ii = -N / 2; ii < N / 2 + 1; ii++) {
				for (int jj = -M / 2; jj < M / 2 + 1; jj++) {
					ConImg += (float)(InImg[(hh + ii) * WIDTH + ww + jj] * mask[ii + N / 2][jj + M / 2]);
				}
			}
		}

		LOG[i] = ConImg;
	}

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		int hh = i / WIDTH;
		int ww = i % WIDTH;
		float p[9] = { 0.0 };
		float ConImg = 0.0;

		if (hh >= N / 2 && hh < HEIGHT - N / 2 && ww >= M / 2 && ww < WIDTH - M / 2) {
			p[0] = LOG[(hh - 1) * WIDTH + ww - 1] * LOG[(hh + 1) * WIDTH + ww + 1];
			p[1] = LOG[(hh - 1) * WIDTH + ww] * LOG[(hh + 1) * WIDTH + ww];
			p[2] = LOG[(hh - 1) * WIDTH + ww + 1] * LOG[(hh + 1) * WIDTH + ww - 1];
			p[3] = LOG[(hh)*WIDTH + ww - 1] * LOG[(hh)*WIDTH + ww + 1];
		}
		
		for (int k = 0; k < 4; k++)
			if (p[k] < 0.0 && fabs(p[k]) > Th) OutImg[i] = 255;

	}

	fp = fopen("lenna_LOG.raw", "wb");
	fwrite(OutImg, 1, WIDTH * HEIGHT, fp);
	fclose(fp);

	return 0;
}