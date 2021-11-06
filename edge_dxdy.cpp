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
#pragma warning(disable:4996)

int main() {
	FILE* fp;
	unsigned char* InImg = NULL;
	float* dy = NULL;
	float* dx = NULL;
	unsigned char* Idy = NULL;
	unsigned char* Idx = NULL;
	unsigned char* OutImg = NULL;

	float mask[N][M] = { {-1.0, -2.0, -1.0},
						  {0.0, 0.0, 0.0},
						  {1.0, 2.0, 1.0} };

	float mask2[N][M] = { {-1.0, 0.0, 1.0},
					  {-2.0, 0.0, 2.0},
					  {-1.0, 0.0, 1.0} };

	InImg = (unsigned char*)malloc(WIDTH * HEIGHT);
	dy = (float*)malloc(WIDTH * HEIGHT * 4);
	dx = (float*)malloc(WIDTH * HEIGHT * 4);
	Idy = (unsigned char*)malloc(WIDTH * HEIGHT);
	Idx = (unsigned char*)malloc(WIDTH * HEIGHT);
	OutImg = (unsigned char*)malloc(WIDTH * HEIGHT);

	memset(InImg, 0, WIDTH * HEIGHT);
	memset(Idy, 0, WIDTH * HEIGHT);
	memset(Idx, 0, WIDTH * HEIGHT);

	fp = fopen("lena_grayscale_374x374.raw", "rb");
	fread(InImg, 1, WIDTH * HEIGHT, fp);
	fclose(fp);



	//OutImg[i] = InImg[i] * mask[N][M]
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

		if (ConImg > TH)ConImg = 255.0;
		else if (ConImg <= TH)ConImg = 0.0;

		Idy[i] = (unsigned char)fabs(ConImg);
		dy[i] = ConImg / L;
	}

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		int hh = i / WIDTH;
		int ww = i % WIDTH;
		float ConImg = 0.0;

		if (hh >= N / 2 && hh < HEIGHT - N / 2 && ww >= M / 2 && ww < WIDTH - M / 2) {
			for (int ii = -N / 2; ii < N / 2 + 1; ii++) {
				for (int jj = -M / 2; jj < M / 2 + 1; jj++) {
					ConImg += (float)(InImg[(hh + ii) * WIDTH + ww + jj] * mask2[ii + N / 2][jj + M / 2]);
				}
			}
		}

		if (ConImg > TH)ConImg = 255.0;
		else if (ConImg <= TH)ConImg = 0.0;
		Idx[i] = (unsigned char)fabs(ConImg);
		dx[i] = ConImg / L;
	}

	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		float ConImg = 0.0;
		ConImg = (unsigned char)sqrt(dy[i] * dy[i] + dx[i] * dx[i]);
		if (ConImg > 255.0)ConImg = 255.0;
		else if (ConImg < 0.0)ConImg = 0.0;
		OutImg[i] = (unsigned char)ConImg;
	}

	fp = fopen("lenna_mask(s).raw", "wb");
	fwrite(OutImg, 1, WIDTH * HEIGHT, fp);
	fclose(fp);

	fp = fopen("lenna_mask(dy).raw", "wb");
	fwrite(Idy, 1, WIDTH * HEIGHT, fp);
	fclose(fp);

	fp = fopen("lenna_mask(dx).raw", "wb");
	fwrite(Idx, 1, WIDTH * HEIGHT, fp);
	fclose(fp);

	return 0;
}

