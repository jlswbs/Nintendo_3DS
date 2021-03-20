// Julia fractal zoomer //


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <3ds.h>

#define WIDTH  400
#define HEIGHT 240
#define SCREEN 400*240


#define ITERATION 256
#define IMG_CONSTANT 0.01
#define REAL_CONSTANT 0.285

	float zoom = 1.0f;


	void Julia(uint16_t size_x, uint16_t size_y, uint16_t offset_x, uint16_t offset_y, uint16_t zoom);


int main(int argc, char **argv) {
	
	gfxInitDefault();

	consoleInit(GFX_BOTTOM, NULL);

	printf("Julia fractal zoomer\n");
	printf(" \n");
	printf("Created by JLS 2021\n");
	printf(" \n");
	printf(" \n");
	printf(" \n");
	printf("START = Exit\n");
	
	srand(svcGetSystemTick());
	
	gfxSetDoubleBuffering(GFX_TOP, true);


	while (aptMainLoop()) {


		gspWaitForVBlank();
			
		hidScanInput();

		if (hidKeysDown() & KEY_START) break;

		Julia(WIDTH, HEIGHT, (WIDTH/2)*zoom, (HEIGHT/2)*zoom, 100/zoom);

		zoom *= 0.9f;

		if(zoom <= 0.01f) zoom = 1.0f;


		gfxFlushBuffers();
		gfxSwapBuffers();

		}

	gfxExit();

	return 0;

}


void Julia(uint16_t size_x, uint16_t size_y, uint16_t offset_x, uint16_t offset_y, uint16_t zoom){

	float tmp1, tmp2;
	float num_real, num_img;
	float radius;
	uint8_t i;
	uint16_t x,y;

	for (y=0; y<size_y; y++){

		for (x=0; x<size_x; x++){

			num_real = y - offset_y;
			num_real = num_real / zoom;
			num_img = x - offset_x;
			num_img = num_img / zoom;
			i=0;
			radius = 0;

			while ((i<ITERATION-1) && (radius < 4)){

				tmp1 = num_real * num_real;
				tmp2 = num_img * num_img;
				num_img = 2*num_real*num_img + IMG_CONSTANT;
				num_real = tmp1 - tmp2 + REAL_CONSTANT;
				radius = tmp1 + tmp2;
				i++;
			}

			u8* fb = gfxGetFramebuffer(GFX_TOP, GFX_LEFT, NULL, NULL);

			fb[3*((y)+(x)*240)]   = i<<1;
			fb[3*((y)+(x)*240)+1] = i<<2;
			fb[3*((y)+(x)*240)+2] = i<<3;

		}
	}
 }